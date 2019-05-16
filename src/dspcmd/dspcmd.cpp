static bool terminated = false;

#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"
#include "config.h"
#include "operations.h"

#include <signal.h>
#include <math.h>

/********************************************************************
Setup signal handling.

This allows us to close down gracefully when the user presses
ctrl+c
********************************************************************/

static void signalIntTermHandler (int signal_value)
{
    HLog("Caught SIGTERM or SIGINT");
    terminated = true;
}

static void SetupSignalHandling()
{
    struct sigaction action;
    action.sa_handler = [](int) { terminated = true; };
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}




int main(int argc, char** argv)
{
    // Show application name and parse input arguments
	std::cout << "dspcmd: using Hardt " + getversion() << std::endl ;
    if( parseArguments(argc, argv) )
    {
        return 0;
    }

    // Initialize the Hardt library, giving a name for logfiles, or if
    // the '-v' switch has been given, let Hardt log directly to stdout
    HInit(std::string("dspcmd"), Config.Verbose);

    // Setup signal handling
    SetupSignalHandling();

    // Run selected operation
    RunOperation(Config);
}
