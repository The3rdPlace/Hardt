#include "hardtapi.h"

std::vector<HSoundcard::DeviceInformation> HSoundcard::GetDeviceInformation()
{
    std::vector<HSoundcard::DeviceInformation> results;

    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        throw new HInitializationException("Initialization error");
    }

    // Get number of devices
    int numDevices;
    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        // No devices, thus an empty list of deviceinfo
        return results;
    }

    // Get default device
    PaDeviceIndex defaultDevice = Pa_GetDefaultInputDevice ();

    // Run through all sound devices available on this system
    const   PaDeviceInfo *deviceInfo;
    for( int i=0; i<numDevices; i++ )
    {
        // Insert new info object
        results.push_back(HSoundcard::DeviceInformation());

        // Get basic info for this device
        deviceInfo = Pa_GetDeviceInfo(i);
        results[i].Device = i;
        results[i].Name = deviceInfo->name;
        results[i].Inputs = deviceInfo->maxInputChannels;
        results[i].Outputs = deviceInfo->maxOutputChannels;
        results[i].IsDefaultDevice = i == defaultDevice;
    }

    // Done, terminate portaudio, ignore errors
    Pa_Terminate();

    // Return the list of information
    return results;
}

int HSoundcard::GetDefaultDevice()
{
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        throw new HInitializationException("Initialization error");
    }

    // Get default device
    PaDeviceIndex defaultDevice = Pa_GetDefaultInputDevice ();

    // Done, terminate portaudio, ignore errors
    Pa_Terminate();

    // Return the device info
    return defaultDevice;
}