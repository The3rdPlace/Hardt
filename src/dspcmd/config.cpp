#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

#include "config.h"
#include "hardtapi.h"

DspCmdConfig Config;

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

bool VerifyConfig()
{
    return false;
}