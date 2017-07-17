#include "soapH.h"
#include "httpget.h"
#include "httppost.h"

class WebServer
{
public:
    WebServer(): mpost_handlers(NULL){};
    ~WebServer(){};

    int  init();
    int  run();
    void end();

    struct soap* getSoap();

private:
    struct soap *msoap;

    struct http_post_handlers *mpost_handlers;

    //int http_GET_handler(struct soap* soap);
    //int http_POST_handler(struct soap* soap);
    //int http_PUT_handler(struct soap* soap);
    //int http_DELETE_handler(struct soap* soap);

    //int CheckAuthorization(struct soap* soap);
};
