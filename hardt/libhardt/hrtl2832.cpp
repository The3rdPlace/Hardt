#include <stdlib.h>
#include <vector>

#include "hrtl2832.h"
#include "hexceptions.h"

std::vector<HRtl2832::DeviceInformation> HRtl2832::GetDeviceInformation()
{
    std::vector<HRtl2832::DeviceInformation> results;

    // Enumerate the existing devices
    char vendor[256], product[256], serial[256];
    int deviceCount = rtlsdr_get_device_count();
	if( deviceCount <= 0 ) {
        HLog("No RTL2832 devices found");
        return results;
	}
	for( int i = 0; i < deviceCount; i++ ) {
		rtlsdr_get_device_usb_strings(i, vendor, product, serial);

        // Get the available gain values
        rtlsdr_dev_t *dev;
        rtlsdr_open(&dev, i);
        if( dev == NULL ) {
            HError("Failed to open RTL-2832 device with index %d while enumerating devices", i);
        } else {
            HLog("RTL-2832 device with index %d opened while enumerating devices", i);

            // Device information
            results.push_back(HRtl2832::DeviceInformation());
            results[i].Device = i;
            results[i].Vendor = vendor;
            results[i].Product = product;
            results[i].Serial = serial;
            results[i].IsDefaultDevice = (i == 0);

            // Available gain values
            int gains = rtlsdr_get_tuner_gains(dev, nullptr);
            HLog("RTL-2832 device with index %d has %d available gain values", i, gains);
            int availableGains[gains];
            rtlsdr_get_tuner_gains(dev, availableGains);
            for( int g = 0; g < gains; g++ ) {
                results[i].Gain.push_back(availableGains[g]);
            }

            // Close device
            rtlsdr_close(dev);
            HLog("RTL-2832 device with index %d closed", i);
        }
	}

    // Return the list of information
    return results;
}

int HRtl2832::GetDefaultDevice()
{
    // There is no notion of a system-default RTL2832 device, so allways return device 0
    return 0;
}

int HRtl2832::AvailableDevices()
{
	return rtlsdr_get_device_count();
}