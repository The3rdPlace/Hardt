#include "hardtapi.h"
#include "dspcmd.h"
#include "signalhandling.h"
#include "config.h"
#include "operations.h"

int main(int argc, char** argv)
{
    // Show application name and parse input arguments
	std::cout << "dspcmd: using Hardt " + getversion() << std::endl ;
    if( parseArguments(argc, argv) )
    {
        return 0;
    }

    // Make sure the configuration is sane
    if( VerifyConfig() )
    {
        return 1;
    }

    // Initialize the Hardt library, giving a name for logfiles, or if
    // the '-v' switch has been given, let Hardt log directly to stdout
    HInit(std::string("dspcmd"), Config.Verbose);

    // Setup signal handling
    SetupSignalHandling();

    // Run selected operation
    return RunOperation();
}
