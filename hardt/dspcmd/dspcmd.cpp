#include "hardtapi.h"
#include "dspcmd.h"
#include "signalhandling.h"
#include "config.h"
#include "operations.h"

int main(int argc, char** argv)
{
    // Initialize the Hardt library, giving a name for logfiles, or if
    // the '-v' switch has been given, let Hardt log directly to stdout.
    // We need to detect the -v switch now to enable verbose logging from
    // the beginning
    for( int i = 0; i < argc; i++ )
    {
        if( strcmp(argv[i], "-v" ) == 0 )
        {
            Config.Verbose = true;
        }
    }
    HInit(std::string("dspcmd"), Config.Verbose);

    // Show application name and parse input arguments
	std::cout << "dspcmd: using Hardt " + getVersion() << std::endl ;
    if( parseArguments(argc, argv) )
    {
        return 0;
    }

    // Make sure the configuration is sane
    if( VerifyConfig() )
    {
        return 1;
    }

    // Setup signal handling
    SetupSignalHandling();

    // Run selected operation
    return RunOperation();
}
