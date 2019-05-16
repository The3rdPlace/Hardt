#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

#include "operations.h"
#include "hardtapi.h"

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

    // Calculate how many blocks fit into 10 seconds
    unsigned long int blocks = (10 * Config.Rate) / Config.Blocksize;

    // Create and run the signalgenerator
    HVfo<T> sg(Config.Rate, Config.Frequency, (T) 15000, Config.Phase);
    HStreamProcessor<T> proc(wr, &sg, Config.Blocksize, &terminated);
    proc.Run(blocks);

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

int RunOperation(DspCmdConfig config)
{
return 0;
}
