#include "soapH.h"
#include "WebServer.h"
#include "RTSPSrcJsonFormat.h"


#define USER_ID "admin"
#define PASSWD  "123n4567"
#define mSec   *-1000

#define PORT       8080
#define LISTENMAX  100

int http_GET_handler(struct soap* soap);
int http_POST_handler(struct soap* soap);
int http_PUT_handler(struct soap* soap);
int http_DELETE_handler(struct soap* soap);

int CheckAuthorization(struct soap* soap);

struct http_post_handlers handlers[]=
{
    {"POST", http_POST_handler},
    {"PUT", http_PUT_handler},
    {"DELETE", http_DELETE_handler},
    {0}
};

int WebServer::init()
{
    msoap = soap_new();
    mpost_handlers = handlers;

    if (soap_register_plugin_arg(msoap, http_get, (void*)http_GET_handler))
    {
        soap_print_fault(msoap, stderr);
        return 1;
    }
    if (soap_register_plugin_arg(msoap, http_post, (void*)mpost_handlers))
    {
        soap_print_fault(msoap, stderr);
        return 1;
    }

    msoap->userid = USER_ID;
    msoap->passwd = PASSWD;
    msoap->accept_timeout = 50 mSec;

    RTSPSrcJsonFormat rtspsrcjson;
    rtspsrcjson.read_from_file();
#if 0
    Json::Value root, aroot;
    Json::Reader reader;
    const char* s = "{\"name\":\"nufront\", \"msg\":[\"hahaha\", \"hehehehe\", \"lalalalalala\"], \"number\":1024, \"subobj\":{\"item1\":\"Beijing\", \"item2\":\"Guangdong\", \"item3\":2}}";
    bool boolean = reader.parse(s, root);
    std::cout << "reader.parse(s, root) return: " << boolean << std::endl;

    printf("root[name].asString().data() = %s\n", root["name"].asString().data());
    printf("root[name].asString().c_str() = %s\n", root["name"].asString().c_str());
    std::cout << "name:" << root["name"].asString() << std::endl;
    Json::Value array = root["msg"];
    for (int i = 0; i < array.size(); i++)
    {
        std::cout << "----" << array[i].asString() << std::endl;
    }
    std::cout << "number:" << root["number"].asInt() << std::endl;
    std::cout << "subobj: item1: " << root["subobj"]["item1"].asString() << std::endl;

    Json::FastWriter fastw;
    Json::StyledWriter styledw;
    std::string str = fastw.write(root);
    std::cout << str << std::endl;
    str = styledw.write(root);
    std::cout << str << std::endl;
#endif
    return 0;
}

void* WebServer::_run(void* wsptr)
{
    WebServer* me = (WebServer*)wsptr;
    SOAP_SOCKET ssock = soap_bind(me->msoap, 0, PORT, LISTENMAX);
    if (!soap_valid_socket(ssock))
    {
        soap_print_fault(me->msoap, stderr);
        return ((void*)1);
    }

    printf("Start accept client!\n");

    while (true)
    {
        pthread_testcancel();

        SOAP_SOCKET ssockclient = soap_accept(me->msoap);
        if (!soap_valid_socket(ssockclient))
        {
            //if (me->msoap->errnum)
            //{
                //soap_print_fault(me->msoap, stderr);
                //printf("Retry...\n");
                //continue;
            //}
            //printf("gSOAP Web server timed out\n");
            continue;
        }
        printf("Client Connect!\n");
        soap_serve(me->msoap);
    }

    return ((void*)0);
}

int WebServer::run()
{
    int r;
    r = pthread_create(&msoapThreadId, NULL, _run, (void*)this);
    if (r != 0)
    {
        printf("webserver start failed!\n");
        return r;
    }
    return 0;
}

void WebServer::end()
{
    pthread_cancel(msoapThreadId);
    pthread_join(msoapThreadId, NULL);
    soap_destroy(msoap);
    soap_end(msoap);
    soap_done(msoap);
}

struct soap* WebServer::getSoap()
{
    return msoap;
}

int http_GET_handler(struct soap* soap)
{
    if (CheckAuthorization(soap))
    {
        return 401;
    }
    printf("http_GET_handler()!  path: %s\n", soap->path);

    std::string id;
    if (!soap_tag_cmp(soap->path, "/device") || !soap_tag_cmp(soap->path, "/device/"))
    {
        id = "all";
    }
    else if (!soap_tag_cmp(soap->path, "/device/*"))
    {
        if (*(soap->path + strlen(soap->path) - 1) == '/')
        {
            *(soap->path + strlen(soap->path) - 1) = 0;
        }
        id = soap->path + 8;
    }
    else
    {
        return 404;
    }

    RTSPSrcJsonFormat rtspsrcjson;
    std::string str;
    if (rtspsrcjson.find_by_id(str, id) == false)
    {
        return 404;
    }
    if (str == "{}\n")
    {
        str = "NULL\n";
    }
    if (soap_send_raw(soap, str.c_str(), str.length()))
    {
        soap_end_send(soap);
        return soap->error;
    }
    return soap_end_send(soap);
}

int http_POST_handler(struct soap* soap)
{
    if (CheckAuthorization(soap))
    {
        return 401;
    }
    printf("http_POST_handler() !\n");

    if (soap_response(soap, SOAP_HTML))
    {
        soap_end_send(soap);
        return soap->error;
    }
    soap_send(soap, "<html>http_POST_handler good</html>");
    soap_end_send(soap);
    return SOAP_OK;
}

int http_PUT_handler(struct soap* soap)
{
    if (CheckAuthorization(soap))
    {
        return 401;
    }
    printf("http_PUT_handler() !\n");

    std::string id;
    if (!soap_tag_cmp(soap->path, "/device") || !soap_tag_cmp(soap->path, "/device/"))
    {
        id = "all";
    }
    else if (!soap_tag_cmp(soap->path, "/device/*"))
    {
        if (*(soap->path + strlen(soap->path) - 1) == '/')
        {
            *(soap->path + strlen(soap->path) - 1) = 0;
        }
        id = soap->path + 8;
    }
    else
    {
        return 404;
    }

    const char *body = soap_get_http_body(soap, NULL);

    RTSPSrcJsonFormat rtspsrcjson;
    std::string str = body;
    if (true == rtspsrcjson.update_object(id, str))
    {
        body = "PUT is OK!";
    }
    else
    {
        body = "PUT is Failed!";
    }
    if (soap_response(soap, SOAP_FILE) || soap_send_raw(soap, body, strlen(body)))
    {
        soap_end_send(soap);
        return soap->error;
    }
    return soap_end_send(soap);
}

int http_DELETE_handler(struct soap* soap)
{
    if (CheckAuthorization(soap))
    {
        return 401;
    }
    printf("http_DELETE_handler() !\n");

    std::string id;
    if (!soap_tag_cmp(soap->path, "/device") || !soap_tag_cmp(soap->path, "/device/"))
    {
        id = "all";
    }
    else if (!soap_tag_cmp(soap->path, "/device/*"))
    {
        if (*(soap->path + strlen(soap->path) - 1) == '/')
        {
            *(soap->path + strlen(soap->path) - 1) = 0;
        }
        id = soap->path + 8;
    }
    else
    {
        return 404;
    }

    RTSPSrcJsonFormat rtspsrcjson;
    const char *reply;
    if (true == rtspsrcjson.delete_object(id))
    {
        reply = "<html>DELETE is OK!</html>";
    }
    else
    {
        reply = "<html>DELETE is failed!</html>";
    }

    if (soap_response(soap, SOAP_HTML))
    {
        soap_end_send(soap);
        return soap->error;
    }
    soap_send(soap, reply);
    return soap_end_send(soap);
}

int CheckAuthorization(struct soap* soap)
{
#if 0
    if (!soap->userid || !soap->passwd ||
        strcmp(soap->userid, USER_ID)  ||
        strcmp(soap->passwd, PASSWD))
    {
        return 401;
    }
#endif
    return 0;
}
