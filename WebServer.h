#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "soapH.h"
#include "httpget.h"
#include "httppost.h"

class WebServer
{
public:
    WebServer(): msoap(NULL), mpost_handlers(NULL), msoapThreadId(0){};
    ~WebServer(){};

    int  init();
    int  run();
    void end();

    struct soap* getSoap();

private:
    static void* _run(void *);

    struct soap *msoap;
    pthread_t msoapThreadId;
    struct http_post_handlers *mpost_handlers;
};

#endif
