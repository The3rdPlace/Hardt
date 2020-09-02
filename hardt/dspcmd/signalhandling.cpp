#include <signal.h>
#include "hardtapi.h"

bool terminated = false;

void SetupSignalHandling()
{
    struct sigaction action;
    action.sa_handler = [](int) {
        HLog("Caught SIGTERM or SIGINT");
        terminated = true;
    };
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}
