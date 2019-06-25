#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <ctime>

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

        Config.IsFileRecorder = argBoolCmp(argv[argNo], "-rf", Config.IsFileRecorder);
        Config.IsFilePlayer = argBoolCmp(argv[argNo], "-pf", Config.IsFilePlayer);

        Config.ForceOverwrite = argBoolCmp(argv[argNo], "-force", Config.ForceOverwrite);

        if( argNo < argc - 1)
        {
            Config.InputFile = argCharCmp(argv[argNo], "-if", argv[argNo + 1], Config.InputFile);
            Config.OutputFile = argCharCmp(argv[argNo], "-of", argv[argNo + 1], Config.OutputFile);

            Config.Blocksize = argIntCmp(argv[argNo], "-bs", argv[argNo + 1], Config.Blocksize);

            Config.InputDevice = argIntCmp(argv[argNo], "-id", argv[argNo + 1], Config.InputDevice);
            Config.OutputDevice = argIntCmp(argv[argNo], "-od", argv[argNo + 1], Config.OutputDevice);
            Config.Rate = argIntCmp(argv[argNo], "-r", argv[argNo + 1], Config.Rate);
            Config.Format = argIntCmp(argv[argNo], "-f", argv[argNo + 1], Config.Format);

            Config.IsNetworkWriterServer = argBoolCmp(argv[argNo], "-ns", Config.IsNetworkWriterServer);
            Config.Port = argIntCmp(argv[argNo], "-ns", argv[argNo + 1], Config.Port);

            Config.FileFormat = argCharCmp(argv[argNo], "-ff", argv[argNo + 1], Config.FileFormat);

            if( argBoolCmp(argv[argNo], "-start", false) )
            {
                Config.Timer.setStart(argv[argNo + 1]);
            }

            if( argBoolCmp(argv[argNo], "-stop", false) )
            {
                Config.Timer.setStop(argv[argNo + 1]);
            }

            Config.Average = argIntCmp(argv[argNo], "-avg", argv[argNo + 1], Config.Average);

            Config.IsFFTMagnitudePlot = argBoolCmp(argv[argNo], "-fmp", Config.IsFFTMagnitudePlot);
            Config.FFTSize = argIntCmp(argv[argNo], "-fmp", argv[argNo + 1], Config.FFTSize);

            Config.IsFFTMagnitudeGnuPlot = argBoolCmp(argv[argNo], "-fmgp", Config.IsFFTMagnitudeGnuPlot);
            Config.FFTSize = argIntCmp(argv[argNo], "-fmgp", argv[argNo + 1], Config.FFTSize);
        }

        if( argNo < argc - 2 )
        {
            Config.IsNetworkReaderClient = argBoolCmp(argv[argNo], "-nc", Config.IsNetworkReaderClient);
            Config.Address = argCharCmp(argv[argNo], "-nc", argv[argNo + 1], Config.Address);
            Config.Port = argIntCmp(argv[argNo], "-nc", argv[argNo + 2], Config.Port);

            Config.XMin = argIntCmp(argv[argNo], "-pr", argv[argNo + 1], Config.XMin);
            Config.XMax = argIntCmp(argv[argNo], "-pr", argv[argNo + 2], Config.XMax);

            Config.IsFileConverter = argBoolCmp(argv[argNo], "-fc", Config.IsFileConverter);
            Config.InFileFormat = argCharCmp(argv[argNo], "-fc", argv[argNo + 1], Config.InFileFormat);
            Config.OutFileFormat = argCharCmp(argv[argNo], "-fc", argv[argNo + 2], Config.OutFileFormat);
        }

        if( argNo < argc - 3 )
        {
            Config.IsSignalGenerator = argBoolCmp(argv[argNo], "-sg", Config.IsSignalGenerator);
            Config.Frequency = argIntCmp(argv[argNo], "-sg", argv[argNo + 1], Config.Frequency);
            PhaseIntValue = argIntCmp(argv[argNo], "-sg", argv[argNo + 2], PhaseIntValue);
            Config.Phase = ((float) PhaseIntValue / 360) * 2 * M_PI;
            Config.Duration = argIntCmp(argv[argNo], "-sg", argv[argNo + 3], Config.Duration);
        }

        if( argNo < argc - 4 )
        {
            Config.XMin = argIntCmp(argv[argNo], "-pr", argv[argNo + 1], Config.XMin);
            Config.XMax = argIntCmp(argv[argNo], "-pr", argv[argNo + 2], Config.XMax);
            Config.YMin = argIntCmp(argv[argNo], "-pr", argv[argNo + 3], Config.YMin);
            Config.YMax = argIntCmp(argv[argNo], "-pr", argv[argNo + 4], Config.YMax);
        }

        if( argBoolCmp(argv[argNo], "-h", false) )
        {
            std::cout << "Usage: dspcmd [option [value]]" << std::endl << std::endl;
            std::cout << "-a                         Show a list of available audio devices" << std::endl;
            std::cout << "-h                         Show this help" << std::endl;
            std::cout << "-v                         Be verbose, dont write to logfiles but to stdout" << std::endl;
            std::cout << std::endl;

            std::cout << "-bs blocksize              Blocksize used by readers and writers (default = 1024)" << std::endl;
            std::cout << "-f                         Sample format (" << H_SAMPLE_FORMAT_INT_8 << "=Int8, " << H_SAMPLE_FORMAT_UINT_8 << "=UInt8, " << H_SAMPLE_FORMAT_INT_16 << "=Int16, " /*<< H_SAMPLE_FORMAT_INT_24 << "=Int24, "*/ << H_SAMPLE_FORMAT_INT_32 << "=Int32) (default " << H_SAMPLE_FORMAT_INT_16 << " = Int16)" << std::endl;
            std::cout << "-ff file|wav               Type of filereader/filewriterwriter" << std::endl;
            std::cout << "-id device                 Input audio device" << std::endl;
            std::cout << "-if name                   Name and path of input file" << std::endl;
            std::cout << "-od device                 Output audio device" << std::endl;
            std::cout << "-of name                   Name and path of output file" << std::endl;
            std::cout << "-force                     Force overwrite of existing file" << std::endl;
            std::cout << "-r rate                    Sample rate (" << H_SAMPLE_RATE_8K << ", " << H_SAMPLE_RATE_11K << ", " << H_SAMPLE_RATE_22K << ", " << H_SAMPLE_RATE_32K << ", " << H_SAMPLE_RATE_44K1 << ", " << H_SAMPLE_RATE_48K << ", " << H_SAMPLE_RATE_96K << ", " << H_SAMPLE_RATE_192K << ") (default 48000)" << std::endl;
            std::cout << "-start [YYYY-MM-DD ]hh:mm  Start time" << std::endl;
            std::cout << "-stop [YYYY-MM-DD ]hh:mm   Stop time" << std::endl;
            std::cout << std::endl;
            std::cout << "-sg freq phase duration    Run as signalgenerator, duration in seconds" << std::endl;
            std::cout << std::endl;

            std::cout << "-nc server port            Run as network client, reading from the network and writing to a local writer" << std::endl;
            std::cout << "-ns port                   Run as network server, reading from a local reader and writing to the network" << std::endl;
            std::cout << std::endl;

            std::cout << "-rf                        Record file" << std::endl;
            std::cout << "-pf                        Play file" << std::endl;
            std::cout << std::endl;

            std::cout << "-fmp size                  Run FFT on a file and plot the magnitude spectrum on screen" << std::endl;
            std::cout << "-fmgp size                 Run FFT on a file and plot the magnitude spectrum on screen using GnuPlot" << std::endl;
            std::cout << "-avg number                Take 'number' FFT's and return the average spectrum" << std::endl;
            std::cout << "-pr xmin xmax              Set minimum and maximum on the x-axis when plotting (only GnuPlot)" << std::endl;
            std::cout << "-pr xmin xmax ymin ymax    Set minimum and maximum on the x-axis when plotting (only GnuPlot)" << std::endl;
            std::cout << std::endl;

            std::cout << "-fc in-format out-format   Convert input file format to output file format" << std::endl;
            std::cout << std::endl;

            // Force exit
            return true;
        }
    }

    // Arguments read, go ahead
    return false;
}

bool VerifyConfig()
{
    // We always needs a samplerate and a sample format
    if( Config.Rate == -1 )
    {
        std::cout << "No rate (-r)" << std::endl;
        return true;
    }
    if( Config.Format == -1 )
    {
        std::cout << "No format (-f)" << std::endl;
        return true;
    }

    // If we are writing a file, check if it exists
    if( Config.OutputFile != NULL && !Config.ForceOverwrite )
    {
        if( !!std::ifstream(Config.OutputFile) )
        {
            std::cout << "Output file exists!. use -force to overwrite" << std::endl;
            return true;
        }
    }

    return false;
}