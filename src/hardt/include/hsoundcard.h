#ifndef __HSOUNDCARD_H
#define __HSOUNDCARD_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

#include <portaudio.h>

// Supported PortAudio sample rates, typed to hide portaudio specific names
#define H_SAMPLE_FORMAT PaSampleFormat
#define H_SAMPLE_FORMAT_INT_8 paInt8
#define H_SAMPLE_FORMAT_UINT_8 paUInt8
#define H_SAMPLE_FORMAT_INT_16 paInt16
//#define H_SAMPLE_FORMAT_INT_24 paInt24  // Need some extra work in the reader before we can enable this
#define H_SAMPLE_FORMAT_INT_32 paInt32

// Samplerates. Not all rates are supported by any given card
#define H_SAMPLE_RATE int
#define H_SAMPLE_RATE_8K 8000
#define H_SAMPLE_RATE_11K 11025
#define H_SAMPLE_RATE_22K 22050
#define H_SAMPLE_RATE_32K 32000
#define H_SAMPLE_RATE_44K1 44100
#define H_SAMPLE_RATE_48K 48000
#define H_SAMPLE_RATE_96K 96000
#define H_SAMPLE_RATE_192K 192000

class HSoundcard
{
    public:

        struct DeviceInformation
        {
            int Device;
            std::string Name;
            int Inputs;
            int Outputs;
            bool IsDefaultDevice;
        };

        static std::vector<DeviceInformation> GetDeviceInformation();
        static int GetDefaultDevice();
};

#endif