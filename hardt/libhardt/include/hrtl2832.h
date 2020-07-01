#ifndef __HRTL_H
#define __HRTL_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

#include "hardt.h"
#include "rtl-sdr.h"

/**
    Base class for RTL2832 readers.
    This class has static functions for obtaining information
    on a given RTL2832 devices in the system.
*/
class HRtl2832
{
    public:

        /**
            Holds information for the existing RTL28332 devices found on the system
        */
        struct DeviceInformation
        {
            /**
                Logical device number. This is the number that must be given to any
                rtl2832 reader to identify the device to use
            */
            int Device;

            /**
                The name of the device vendor
            */
            std::string Vendor;

            /**
                The product name (device name)
            */
            std::string Product;

            /**
                The product serial number
            */
            std::string Serial;

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