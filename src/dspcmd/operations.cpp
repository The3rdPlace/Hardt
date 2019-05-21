#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

#include "operations.h"
#include "signalhandling.h"
#include "config.h"
#include "hardtapi.h"

template <typename T>
int RunNetworkWriterServer()
{
    HSoundcardReader<T> rdr(Config.InputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    HNetworkProcessor<T> srv(Config.Port, &rdr, Config.Blocksize, &terminated);
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
int RunNetworkReaderClient()
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
int RunSignalGenerator()
{
    // Create requested writer
    HWriter<T>* wr;
    if( Config.OutputDevice != -1 )
    {
        wr = new HSoundcardWriter<T>(Config.OutputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    }
    else if ( Config.OutputFile != NULL )
    {
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
    }
    else
    {
        std::cout << "No output file or device" << std::endl;
        return -1;
    }

    // Calculate how many blocks fit into 10 seconds
    unsigned long int blocks = (Config.Duration * Config.Rate) / Config.Blocksize;

    // Create and run the signalgenerator
    HVfo<T> sg(Config.Rate, Config.Frequency, (T) 15000, Config.Phase);
    HStreamProcessor<T> proc(wr, &sg, Config.Blocksize, &terminated);
    proc.Run(blocks);

    // Delete writer
    if( Config.OutputDevice != -1 )
    {
        delete (HSoundcardWriter<T>*) wr;
    }
    else
    {
        if( strcmp(Config.FileFormat, "wav") == 0 )
        {
            delete (HWavWriter<T>*) wr;
        }
        else if( strcmp(Config.FileFormat, "file") == 0 )
        {
            delete (HFileWriter<T>*) wr;
        }
    }

    return 0;
}

template <typename T>
int RunFilePlayer()
{
    // We need an output device and an input file

    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "file") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HSoundcardWriter<T> wr(Config.OutputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(&wr, rd, Config.Blocksize, &terminated);
    proc.Run();

    // Delete reader
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.FileFormat, "file") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }

    return 0;
}

template <typename T>
int RunOperation()
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
            std::cout << "No Server address (-nr server port)" << std::endl;
            return 1;
        }
        if( Config.Port == -1 )
        {
            std::cout << "No Server port (-nr server port)" << std::endl;
            return 1;
        }

        return RunNetworkReaderClient<T>();
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
        if( Config.Port == -1 )
        {
            std::cout << "No Server port (-ns port)" << std::endl;
            return 1;
        }

        return RunNetworkWriterServer<T>();
    }

    // Generator a single tone
    if( Config.IsSignalGenerator )
    {
        // Verify configuration
        if( Config.Frequency == 0 )
        {
            std::cout << "Frequency is 0, not possible" << std::endl;
            return 1;
        }

        return RunSignalGenerator<T>();
    }

    // Play a local file
    if( Config.IsFilePlayer )
    {
        // Verify configuration
        if( Config.OutputDevice == -1 )
        {
            std::cout << "No output device" << std::endl;
            return -1;
        }
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file" << std::endl;
            return -1;
        }

        return RunFilePlayer<T>();
    }

    // No known operation could be determined from the input arguments
    std::cout << "Sorry, operation could not be determined from input arguments" << std::endl;
    std::cout << "Try 'dspcmd -h'" << std::endl;
	return 1;
}

int RunOperation()
{
    // Two stage operation calling, first find out which datatype is needed
    try
    {
        int rc;
        switch(Config.Format)
        {
            case H_SAMPLE_FORMAT_INT_8:
                HLog("Base datatype is int8_t");
                return RunOperation<int8_t>();
            case H_SAMPLE_FORMAT_UINT_8:
                HLog("Base datatype is uint8_t");
                return RunOperation<uint8_t>();
            case H_SAMPLE_FORMAT_INT_16:
                HLog("Base datatype is int16_t");
                return RunOperation<int16_t>();
            /*case H_SAMPLE_FORMAT_INT_24:
                HLog("Base datatype is int24_t");
                return RunOperation<int8_t>();*/
            case H_SAMPLE_FORMAT_INT_32:
                HLog("Base datatype is int32_t");
                return RunOperation<int32_t>();
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
