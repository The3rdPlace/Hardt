#ifndef __HSOUNDCARD_H
#define __HSOUNDCARD_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

#include <portaudio.h>

#include "hardt.h"
#include "hexceptions.h"

/**
    Base class for soundcard readers and writers.
    This class has static functions for obtaining information
    on soundcards in the system.
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

        /**
            Get the PortAudio specific sample format identifier
        */
        static PaSampleFormat GetPortAudioFormat(H_SAMPLE_FORMAT format) {
            switch( format ) {
                case H_SAMPLE_FORMAT_INT_8:
                    return paInt8;
                case H_SAMPLE_FORMAT_UINT_8:
                    return paUInt8;
                case H_SAMPLE_FORMAT_INT_16:
                    return paInt16;
                //case H_SAMPLE_FORMAT_INT_24:
                    //return paInt24;
                case H_SAMPLE_FORMAT_INT_32:
                    return paInt32;
            }
            throw new HInitializationException("Unknown sample format given as parameter to HSoundcard::GetPortAudioFormat");
        }
};

#endif