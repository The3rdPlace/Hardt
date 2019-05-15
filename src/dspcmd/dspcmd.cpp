#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"

#include <signal.h>
#include <math.h>

/********************************************************************
Setup signal handling.

This allows us to close down gracefully when the user presses
ctrl+c
********************************************************************/

static bool terminated = false;
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

/********************************************************************
Argument parsing and global configuration
********************************************************************/

// Settings
struct DspCmdConfig
{
    bool Verbose = false;
    bool ShowAudioDevices = false;

    bool IsNetworkReaderClient = false;
    bool IsNetworkWriterClient = false; // not implemented
    bool IsNetworkReaderServer = false; // not implemented
    bool IsNetworkWriterServer = false;
    bool IsSignalGenerator = false;

    int Port = 8080;
    char *Address = NULL;

    char *InputFile = NULL;
    char *OutputFile = NULL;
    char* FileFormat = NULL;

    int InputDevice = -1; // not implemented
    int OutputDevice = -1;
    int Rate = -1;
    int Format = -1;

    int Blocksize = 1024;

    int Frequency;
    float Phase;
} Config;

bool argBoolCmp(const char* arg, const char* option, bool currentValue)
{
    return strcmp(arg, option) == 0 ? true : currentValue;
}

char* argCharCmp(const char* arg, const char* option, char* value, char* currentValue)
{
    return strcmp(arg, option) == 0 ? value : currentValue;
}

int argIntCmp(const char* arg, const char* option, char* value, int currentValue)
{
    return strcmp(arg, option) == 0 ? atoi(value) : currentValue;
}

bool parseArguments(int argc, char** argv)
{
    int PhaseIntValue = 0;

    for( int argNo = 1; argNo < argc; argNo++ )
    {
        Config.Verbose = argBoolCmp(argv[argNo], "-v", Config.Verbose);
        Config.ShowAudioDevices = argBoolCmp(argv[argNo], "-a", Config.ShowAudioDevices);

        if( argNo < argc - 1)
        {
            //Config.InputFile = argCharCmp(argv[argNo], "-if", argv[argNo + 1], Config.InputFile);
            Config.OutputFile = argCharCmp(argv[argNo], "-of", argv[argNo + 1], Config.OutputFile);

            Config.Blocksize = argIntCmp(argv[argNo], "-bs", argv[argNo + 1], Config.Blocksize);

            Config.InputDevice = argIntCmp(argv[argNo], "-id", argv[argNo + 1], Config.InputDevice);
            //Config.OutputDevice = argIntCmp(argv[argNo], "-od", argv[argNo + 1], Config.OutputDevice);
            Config.Rate = argIntCmp(argv[argNo], "-r", argv[argNo + 1], Config.Rate);
            Config.Format = argIntCmp(argv[argNo], "-f", argv[argNo + 1], Config.Format);

            Config.IsNetworkWriterServer = argBoolCmp(argv[argNo], "-sw", Config.IsNetworkWriterServer);
            Config.Port = argIntCmp(argv[argNo], "-sw", argv[argNo + 1], Config.Port);

            //Config.IsNetworkReaderServer = argBoolCmp(argv[argNo], "-sr", Config.IsNetworkReaderServer);
            //Config.Port = argIntCmp(argv[argNo], "-sr", argv[argNo + 1], Config.Port);

            Config.FileFormat = argCharCmp(argv[argNo], "-ff", argv[argNo + 1], Config.FileFormat);
        }

        if( argNo < argc - 2 )
        {
            //Config.IsNetworkWriterClient = argBoolCmp(argv[argNo], "-cw", Config.IsNetworkWriterClient);
            //Config.Server = argIntCmp(argv[argNo], "-cw", argv[argNo + 1], Config.Server);
            //Config.Port = argIntCmp(argv[argNo], "-cw", argv[argNo + 2], Config.Port);

            Config.IsNetworkReaderClient = argBoolCmp(argv[argNo], "-cr", Config.IsNetworkReaderClient);
            Config.Address = argCharCmp(argv[argNo], "-cr", argv[argNo + 1], Config.Address);
            Config.Port = argIntCmp(argv[argNo], "-cr", argv[argNo + 2], Config.Port);

            Config.IsSignalGenerator = argBoolCmp(argv[argNo], "-sg", Config.IsSignalGenerator);
            Config.Frequency = argIntCmp(argv[argNo], "-sg", argv[argNo + 1], Config.Frequency);
            PhaseIntValue = argIntCmp(argv[argNo], "-sg", argv[argNo + 2], PhaseIntValue);
            Config.Phase = ((float) PhaseIntValue / 360) * 2 * M_PI;
        }

        if( argBoolCmp(argv[argNo], "-h", false) )
        {
            std::cout << "Usage: dspcmd [option [value]]" << std::endl << std::endl;
            std::cout << "-a                   Show a list of available audio devices" << std::endl;
            std::cout << "-bs blocksize        Blocksize used by readers and writers (default = 1024)" << std::endl;
            std::cout << "-f                   Sample format (" << H_SAMPLE_FORMAT_INT_8 << "=Int8, " << H_SAMPLE_FORMAT_UINT_8 << "=UInt8, " << H_SAMPLE_FORMAT_INT_16 << "=Int16, " /*<< H_SAMPLE_FORMAT_INT_24 << "=Int24, "*/ << H_SAMPLE_FORMAT_INT_32 << "=Int32)" << std::endl;
            std::cout << "-ff file|wav         Type of filereader/filewriterwriter" << std::endl;
            std::cout << "-id device           Input audio device" << std::endl;
            //std::cout << "-if name           Name and path of input file" << std::endl;
            std::cout << "-h                   Show this help" << std::endl;
            //std::cout << "-od device         Output audio device" << std::endl;
            std::cout << "-of name             Name and path of output file" << std::endl;
            std::cout << "-r rate              Sample rate (" << H_SAMPLE_RATE_8K << ", " << H_SAMPLE_RATE_11K << ", " << H_SAMPLE_RATE_22K << ", " << H_SAMPLE_RATE_32K << ", " << H_SAMPLE_RATE_44K1 << ", " << H_SAMPLE_RATE_48K << ", " << H_SAMPLE_RATE_96K << ", " << H_SAMPLE_RATE_192K << ")" << std::endl;
            std::cout << "-v                   Be verbose, dont write to logfiles but to stdout" << std::endl;
            std::cout << std::endl;

            std::cout << "-cr server port      Run as network client, reading from the network and writing to a local writer" << std::endl;
            //std::cout << "-cw server port    Run as network client, reading from a local reader and writing to the network" << std::endl;
            std::cout << "-sw port             Run as network server, reading from a local reader and writing to the network" << std::endl;
            //std::cout << "-sr port           Run as network server, reading from the network and writing to a local writer" << std::endl;
            std::cout << "-sg frequency phase  Run as signalgenerator" << std::endl;
            // Force exit
            return true;
        }
    }

    // Arguments read, go ahead
    return false;
}

template <typename T>
int RunNetworkWriterServer(DspCmdConfig config)
{
    HSoundcardReader<T> rdr(config.InputDevice, config.Rate, 1, config.Format, config.Blocksize);
    HNetworkProcessor<T> srv(config.Port, &rdr, config.Blocksize, &terminated);
    try
    {
        srv.Run();
        std::cout << rdr.GetMetrics("HSoundcardReader");
        std::cout << srv.GetMetrics("HNetworkProcessor");
    }
    catch( std::exception ex )
    {
        std::cout << "Caught exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

template <typename T>
int RunNetworkReaderClient(DspCmdConfig config)
{
    // Create requested writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "file") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create a network processor and run the client
    HNetworkProcessor<T> client(Config.Address, Config.Port, wr, Config.Blocksize, &terminated);
    try
    {
        client.Run();
    }
    catch( std::exception ex )
    {
        std::cout << "Caught exception: " << ex.what() << std::endl;
        return 1;
    }

    // Delete writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "file") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }
    return 0;
}

template <typename T>
int RunSignalGenerator(DspCmdConfig config)
{
    std::cout << "GENERATOR" << std::endl;

    // Create requested writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "file") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create and run the signalgenerator
    HVfo<T> sg(Config.Rate, Config.Frequency, (T) 15000, Config.Phase);
    HStreamProcessor<T> proc(wr, &sg, Config.Blocksize, &terminated);
    proc.Run();

    // Delete writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "file") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }
    return 0;
}

template <typename T>
int RunOperation(DspCmdConfig config)
{
    // Show audio device information
    if( Config.ShowAudioDevices ) {

        if( HSoundcard::AvailableDevices() == 0 )
        {
            std::cout << "There is no soundcards available on this system" << std::endl;
            return 0;
        }

        std::cout << "Default device is: " << HSoundcard::GetDefaultDevice() << std::endl;
        std::cout << std::endl;

        std::vector<HSoundcard::DeviceInformation> info = HSoundcard::GetDeviceInformation();
        for( std::vector<HSoundcard::DeviceInformation>::iterator it = info.begin(); it != info.end(); it++)
        {
            std::cout << "Device: " << (*it).Device << std::endl;
            std::cout << "Device: " << (*it).Name << std::endl;
            std::cout << "Inputs: " << (*it).Inputs << std::endl;
            std::cout << "Outputs: " << (*it).Outputs << std::endl;
            std::cout << "Is default device: " << ((*it).IsDefaultDevice ? "yes" : "no") << std::endl;
            std::cout << std::endl;
        }
        return 0;
    }

    // Read from network and write to local file
    if( Config.IsNetworkReaderClient )
    {
        // Verify configuration
        if( Config.Address == NULL )
        {
            std::cout << "No output filename (-of)" << std::endl;
            return 1;
        }
        if( Config.Rate == -1 )
        {
            std::cout << "No rate (-r)" << std::endl;
            return 1;
        }
        if( Config.Format == -1 )
        {
            std::cout << "No format (-f)" << std::endl;
            return 1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output filename (-f)" << std::endl;
            return 1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No output file format (-ff)" << std::endl;
            return 1;
        }

        return RunNetworkReaderClient<T>(Config);
    }

    // Read from soundcard and write to network
    if( Config.IsNetworkWriterServer)
    {
        // Verify configuration
        if( Config.InputDevice == -1 )
        {
            std::cout << "No inputdevice (-id)" << std::endl;
            return 1;
        }
        if( Config.Rate == -1 )
        {
            std::cout << "No rate (-r)" << std::endl;
            return 1;
        }
        if( Config.Format == -1 )
        {
            std::cout << "No format (-f)" << std::endl;
            return 1;
        }

        return RunNetworkWriterServer<T>(Config);
    }

    // Generator a single tone
    if( Config.IsSignalGenerator )
    {
        // Verify configuration
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file format (-ff)" << std::endl;
            return 1;
        }
        if( Config.Frequency == 0 )
        {
            std::cout << "Frequency is 0, not possible" << std::endl;
            return 1;
        }
        if( Config.Rate == -1 )
        {
            std::cout << "No rate (-r)" << std::endl;
            return 1;
        }
        if( Config.Format == -1 )
        {
            std::cout << "No format (-f)" << std::endl;
            return 1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No output file format (-ff)" << std::endl;
            return 1;
        }

        return RunSignalGenerator<T>(Config);
    }

    // No known operation could be determined from the input arguments
    std::cout << "Sorry, operation could not be determined from input arguments" << std::endl;
    std::cout << "Try 'dspcmd -h'" << std::endl;
	return 1;

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

    // Two stage operation calling, first find out which datatype is needed
    try
    {
        int rc;
        switch(Config.Format)
        {
            case H_SAMPLE_FORMAT_INT_8:
                HLog("Base datatype is int8_t");
                return RunOperation<int8_t>(Config);
            case H_SAMPLE_FORMAT_UINT_8:
                HLog("Base datatype is uint8_t");
                return RunOperation<uint8_t>(Config);
            case H_SAMPLE_FORMAT_INT_16:
                HLog("Base datatype is int16_t");
                return RunOperation<int16_t>(Config);
            /*case H_SAMPLE_FORMAT_INT_24:
                HLog("Base datatype is int24_t");
                return RunOperation<int8_t>(Config);*/
            case H_SAMPLE_FORMAT_INT_32:
                HLog("Base datatype is int32_t");
                return RunOperation<int32_t>(Config);
            default:
                std::cout << "Unknown sample format " << Config.Format << std::endl;
                return -1;
        }
    }
    catch( HException* hex )
    {
        std::cout << "Caught exception:" << std::endl;
        std::cout << "  " << hex->what() << std::endl;
        return 1;
    }
    catch( std::exception* ex )
    {
        std::cout << "Caught external exception:" << std::endl;
        std::cout << "  " << ex->what() << std::endl;
        return 2;
    }
    catch( ... )
    {
        std::cout << "Unexpected exception, no further information" << std::endl;
        return 3;
    }
}
