#ifndef __HSOUNDCARD_H
#define __HSOUNDCARD_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

#include <portaudio.h>

/**
    Supported PortAudio sample rates, typed to hide portaudio specific names
*/
#define H_SAMPLE_FORMAT PaSampleFormat
#define H_SAMPLE_FORMAT_INT_8 paInt8
#define H_SAMPLE_FORMAT_UINT_8 paUInt8
#define H_SAMPLE_FORMAT_INT_16 paInt16
//#define H_SAMPLE_FORMAT_INT_24 paInt24  // Need some extra work in the reader before we can enable this
#define H_SAMPLE_FORMAT_INT_32 paInt32

/**
    Samplerates. Not all rates are supported by any given card
*/
#define H_SAMPLE_RATE int
#define H_SAMPLE_RATE_8K 8000
#define H_SAMPLE_RATE_11K 11025
#define H_SAMPLE_RATE_22K 22050
#define H_SAMPLE_RATE_32K 32000
#define H_SAMPLE_RATE_44K1 44100
#define H_SAMPLE_RATE_48K 48000
#define H_SAMPLE_RATE_96K 96000
#define H_SAMPLE_RATE_192K 192000

/**
    Base class for soundcard readers and writers.
    This class be instantiated directly to obtain information
    on a given soundcard in the system.
*/
class HSoundcard
{
    public:

        /**
            Holds information on a soundcards capabilities
        */
        struct DeviceInformation
        {
            /**
                Logical device number. This is the number that must be given to any
                soundcard reader or writer to identify the card and device to use
            */
            int Device;

            /**
                The name of the device
            */
            std::string Name;

            /**
                Number of available input channels on this device
            */
            int Inputs;

            /**
                Number of available output channels on this device
            */
            int Outputs;

            /**
                Set to true if this device is the systems default device
            */
            bool IsDefaultDevice;
        };

        /**
            Get the number of devices available
        */
        static int AvailableDevices();

        /**
            Get a list of devices
        */
        static std::vector<DeviceInformation> GetDeviceInformation();

        /**
            Get the device number for the default device
        */
        static int GetDefaultDevice();
};

#endif