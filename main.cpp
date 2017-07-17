#include "WebServer.h"

int main()
{
    WebServer ws;
    ws.init();
    ws.run();
    ws.end();

    return 0;
}
