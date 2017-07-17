#include "soapH.h"

//#include "json.h"
//#include "json-forwards.h"

#include "WebServer.h"

#define USER_ID "admin"
#define PASSWD  "123n4567"

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
    printf("-----%s:%d\n", __func__, __LINE__);

    msoap->userid = USER_ID;
    msoap->passwd = PASSWD;

    return 0;
}

int WebServer::run()
{
    SOAP_SOCKET ssock = soap_bind(msoap, 0, PORT, LISTENMAX);
    if (!soap_valid_socket(ssock))
    {
        soap_print_fault(msoap, stderr);
        return 1;
    }

    printf("Start accept client!\n");

    while (1)
    {
        SOAP_SOCKET ssockclient = soap_accept(msoap);
        if (!soap_valid_socket(ssockclient))
        {
            if (msoap->errnum)
            {
                soap_print_fault(msoap, stderr);
                printf("Retry...\n");
                continue;
            }
            printf("gSOAP Web server timed out\n");
            break;
        }
        printf("Client Connect!\n");
        soap_serve(msoap);
    }

    return 0;
}

void WebServer::end()
{
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

#if 0
    if (strchr(soap->path + 1, '/') || strchr(soap->path + 1, '\\'))
        return 403;
    if (!soap_tag_cmp(soap->path, "*.xml"))
        return copy_file(soap, soap->path + 1, "text/xml");
    if (!soap_tag_cmp(soap->path, "*.json"))
        return copy_file(soap, soap->path + 1, "text/json");

    return 404;
#endif
    if (soap_response(soap, SOAP_HTML))
    {
        soap_end_send(soap);
        return soap->error;
    }
    soap_send(soap, "<html>http_GET_handler good !!!</html>");
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
#if 0
    if (strchr(soap->path + 1, '/') || strchr(soap->path + 1, '\\'))
        return 403;

    if (!soap_tag_cmp(soap->path+1, "easydarwin.json"))
        return update_json_file(soap, "easydarwin.json");
    return save_file(soap, soap->path+1);
#endif
    if (soap_response(soap, SOAP_HTML))
    {
        soap_end_send(soap);
        return soap->error;
    }
    soap_send(soap, "<html>http_PUT_handler good !!!</html>");
    return soap_end_send(soap);
}

int http_DELETE_handler(struct soap* soap)
{
    if (CheckAuthorization(soap))
    {
        return 401;
    }
    printf("http_DELETE_handler() !\n");
#if 0
    if (strchr(soap->path + 1, '/') || strchr(soap->path + 1, '\\'))
        return 403;

    return remove_file(soap, soap->path+1);
#endif
    if (soap_response(soap, SOAP_HTML))
    {
        soap_end_send(soap);
        return soap->error;
    }
    soap_send(soap, "<html>http_DELETE_handler good !!!</html>");
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
