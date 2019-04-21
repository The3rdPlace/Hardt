#include "hsoundcard.h"

void HSoundcard::GetDeviceInformation()
{
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        printf("Initialization error\n");
        return;
    }

    int numDevices;
    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf("No devices\n");
        return;
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
        return;
    }
}
