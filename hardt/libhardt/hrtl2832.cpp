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
        results.push_back(HRtl2832::DeviceInformation());
        results[i].Device = i;
        results[i].Vendor = vendor;
        results[i].Product = product;
        results[i].Serial = serial;
        results[i].IsDefaultDevice = (i == 0);
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