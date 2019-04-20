#include <stdio.h>
#include <iostream>
#include <string.h>

#include <cstdlib>
#include <string>
#include <unistd.h>


// SERVER
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

// CLIENT
#include <arpa/inet.h>


#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"




/********************************************************************
Setup signal handling.

This allows us to close down gracefully when the user presses
ctrl+c
********************************************************************/

static bool terminated = false;
static void signalIntTermHandler (int signal_value)
{
    terminated = true;
    std::cout << "SIGTERM or SITINT" << std::endl;
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
    int Port = 8080;
    char *Address = NULL;

    char *InputFile = NULL;
    char *OutputFile = NULL; // not implemented

    int InputDevice = -1; // not implemented
    int OutputDevice = -1;
    int Rate = -1;
    int Format = -1;

} Config;

bool argBoolCmp(const char* arg, const char* option, bool currentValue)
{
    return strncmp(arg, option, strlen(option)) == 0 ? true : currentValue;
}

char* argCharCmp(const char* arg, const char* option, char* value, char* currentValue)
{
    return strncmp(arg, option, strlen(option)) == 0 ? value : currentValue;
}

int argIntCmp(const char* arg, const char* option, char* value, int currentValue)
{
    return strncmp(arg, option, strlen(option)) == 0 ? atoi(value) : currentValue;
}

void parseArguments(int argc, char** argv)
{
    for( int argNo = 1; argNo < argc; argNo++ )
    {
        Config.Verbose = argBoolCmp(argv[argNo], "-v", Config.Verbose);
        Config.ShowAudioDevices = argBoolCmp(argv[argNo], "-a", Config.ShowAudioDevices);

        if( argNo < argc - 1 )
        {
            //Config.InputFile = argCharCmp(argv[argNo], "-if", argv[argNo + 1], Config.InputFile);
            Config.OutputFile = argCharCmp(argv[argNo], "-of", argv[argNo + 1], Config.OutputFile);

            Config.InputDevice = argIntCmp(argv[argNo], "-id", argv[argNo + 1], Config.InputDevice);
            //Config.OutputDevice = argIntCmp(argv[argNo], "-od", argv[argNo + 1], Config.OutputDevice);
            Config.Rate = argIntCmp(argv[argNo], "-r", argv[argNo + 1], Config.Rate);
            Config.Format = argIntCmp(argv[argNo], "-f", argv[argNo + 1], Config.Format);

            Config.IsNetworkWriterServer = argBoolCmp(argv[argNo], "-sw", Config.IsNetworkWriterServer);
            Config.Port = argIntCmp(argv[argNo], "-sw", argv[argNo + 1], Config.Port);

            //Config.IsNetworkReaderServer = argBoolCmp(argv[argNo], "-sr", Config.IsNetworkReaderServer);
            //Config.Port = argIntCmp(argv[argNo], "-sr", argv[argNo + 1], Config.Port);
        }

        if( argNo < argc - 2 )
        {
            //Config.IsNetworkWriterClient = argBoolCmp(argv[argNo], "-cw", Config.IsNetworkWriterClient);
            //Config.Server = argIntCmp(argv[argNo], "-cw", argv[argNo + 1], Config.Server);
            //Config.Port = argIntCmp(argv[argNo], "-cw", argv[argNo + 2], Config.Port);

            Config.IsNetworkReaderClient = argBoolCmp(argv[argNo], "-cr", Config.IsNetworkReaderClient);
            Config.Address = argCharCmp(argv[argNo], "-cr", argv[argNo + 1], Config.Address);
            Config.Port = argIntCmp(argv[argNo], "-cr", argv[argNo + 2], Config.Port);
        }

        if( argBoolCmp(argv[argNo], "-h", false) )
        {
            std::cout << "Usage: dspcmd [option [value]]" << std::endl << std::endl;
            std::cout << "-a                 Show a list of available audio devices" << std::endl;
            std::cout << "-f                 Sample format (" << H_SAMPLE_FORMAT_INT_8 << "=Int8, " << H_SAMPLE_FORMAT_UINT_8 << "=UInt8, " << H_SAMPLE_FORMAT_INT_16 << "=Int16, " /*<< H_SAMPLE_FORMAT_INT_24 << "=Int24, "*/ << H_SAMPLE_FORMAT_INT_32 << "=Int32)" << std::endl;
            std::cout << "-id device         Input audio device" << std::endl;
            //std::cout << "-if name           Name and path of input file" << std::endl;
            std::cout << "-h                 Show this help" << std::endl;
            //std::cout << "-od device         Output audio device" << std::endl;
            std::cout << "-of name           Name and path of output file" << std::endl;
            std::cout << "-r rate            Sample rate (" << H_SAMPLE_RATE_8K << ", " << H_SAMPLE_RATE_11K << ", " << H_SAMPLE_RATE_22K << ", " << H_SAMPLE_RATE_32K << ", " << H_SAMPLE_RATE_44K1 << ", " << H_SAMPLE_RATE_48K << ", " << H_SAMPLE_RATE_96K << ", " << H_SAMPLE_RATE_192K << ")" << std::endl;
            std::cout << "-v                 Be verbose, dont write to logfiles but to stdout" << std::endl;
            std::cout << std::endl;

            std::cout << "-cr server port    Run as network client, reading from the network and writing to a local writer" << std::endl;
            //std::cout << "-cw server port    Run as network client, reading from a local reader and writing to the network" << std::endl;
            std::cout << "-sw port           Run as network server, reading from a local reader and writing to the network" << std::endl;
            //std::cout << "-sr port           Run as network server, reading from the network and writing to a local writer" << std::endl;
        }
    }
}


int main(int argc, char** argv)
{
    // Show application name and parse input arguments
	std::cout << "dspcmd: using Hardt " + getversion() << std::endl ;
    parseArguments(argc, argv);

    // Initialize the Hardt library, giving a name for logfiles, or if
    // the '-v' switch has been given, let Hardt log directly to stdout
    Config.Verbose ? HInit() : HInit(std::string("dspcmd"));

    // Setup signal handling
    SetupSignalHandling();

    /* Todo: Move this to a component */
    if( Config.ShowAudioDevices ) {

        PaError err = Pa_Initialize();
    	if( err != paNoError )
    	{
    		printf("Initialization error\n");
    		return 1;
    	}

    	int numDevices;
        numDevices = Pa_GetDeviceCount();
        if( numDevices < 0 )
        {
            printf("No devices\n");
            return 2;
        }
        printf("Number of devices: %d\n", numDevices);

        PaDeviceIndex x = Pa_GetDefaultInputDevice ();
        std::cout << "Default device is: " << x << std::endl;


        const   PaDeviceInfo *deviceInfo;
        for( int i=0; i<numDevices; i++ )
        {
            deviceInfo = Pa_GetDeviceInfo(i);
            double desiredSampleRate;
            PaStreamParameters testParameters;
            testParameters.device = i;
            testParameters.channelCount = 1;
            testParameters.sampleFormat = paInt16;
            testParameters.suggestedLatency = Pa_GetDeviceInfo(i)->defaultLowInputLatency ;
            testParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

            std::cout << "==========================================" << std::endl;
            std::cout << "Device: " << i << " = " << deviceInfo->name << std::endl;
            std::cout << "Input: " << deviceInfo->maxInputChannels << " channels" << std::endl;
            std::cout << "Output: " << deviceInfo->maxOutputChannels << " channels" << std::endl;

            int rates[] = { 8000, 11025, 22050, 32000, 44100, 48000 };
            bool gotOne = false;
            for( int j = 0; j < 6; j++)
            {
                PaError err;
                err = Pa_IsFormatSupported( &testParameters, NULL, rates[j] );
                if( err == paFormatIsSupported )
                {
                   std::cout << (j > 0 ? ", " : "") << rates[j];
                   gotOne = true;
                }
            }
            if( !gotOne )
            {
                std::cout << "(no supported rates)";
            }
            std::cout << std::endl;
        }


        err = Pa_Terminate();
        if ( err != paNoError )
        {
            printf("Termination error\n");
            return 3;
        }


        return 0;

    }

    if( Config.IsNetworkReaderClient ) {
        HFileWriter<int> wr("tmp.raw");
        HNetworkClient<int> client = HNetworkClient<int>(Config.Address, Config.Port, &wr, &terminated);
        try
        {
            client.Run();
        }
        catch( std::exception ex )
        {
            std::cout << "Caught exception: " << ex.what() << std::endl;
        }
    }

    if( Config.IsNetworkWriterServer)
    {
        HSoundcardReader<int> rdr(Config.InputDevice, Config.Rate, 1, Config.Format, 1024);
        HNetworkServer<int> srv = HNetworkServer<int>(Config.Port, &rdr, &terminated);
        try
        {
            srv.Run();
        }
        catch( std::exception ex )
        {
            std::cout << "Caught exception: " << ex.what() << std::endl;
        }
    }

    /* Todo: Move this to a component
    Generator* g = new Generator();
    int freqs[1];
    freqs[0] = 25;
    std::complex<double>* samples = g->GetSamples(1000, 1, 1, freqs, 1);
    //print_array< std::complex<double> >(samples, 1000);
    delete g;*/

	return 0;
}

