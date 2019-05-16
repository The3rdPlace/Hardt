#include <signal.h>
#include "hardtapi.h"

static bool terminated = false;

static void signalIntTermHandler (int signal_value)
{
    HLog("Caught SIGTERM or SIGINT");
    terminated = true;
}

void SetupSignalHandling()
{
    struct sigaction action;
    action.sa_handler = [](int) { terminated = true; };
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}
