#include <signal.h>
#include "WebServer.h"

void stop_ws(int);
bool isExit = false;

static WebServer ws;

int main()
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = stop_ws;
    sigaction(SIGINT, &act, NULL);

    ws.init();
    ws.run();

    while (!isExit)
    //while (1)
    {
        int i = 10;
        printf("------%s: I will sleep %ds\n", __func__, i);
        sleep(i);
    }
    printf("######\n");
    return 0;
}

void stop_ws(int sig)
{
    printf("\n---Catch a signal: ");
    if (sig == SIGINT)
    {
        printf("SIGINT\n");
        ws.end();
        isExit = true;
    }
}
