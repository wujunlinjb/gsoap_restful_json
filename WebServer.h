#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

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
};

#endif
