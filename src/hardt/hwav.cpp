#include "hwav.h"
#include <cstring>

HWav::HWav(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, size_t size):
    _filename(filename)
{
    // Copy the header initializer data into the header member
    memcpy((void*) &_header, (void*) emptyHeader, sizeof(WavHeader));

    // Initialize header
    // Todo: Set all fields that differs according to format and rate
}

HWav::HWav(const char* filename):
    _filename(filename)
{
    // Todo: Read header from the named file
}

H_SAMPLE_FORMAT HWav::GetFormat()
{
    return H_SAMPLE_FORMAT_INT_32;
}

int HWav::GetChannels()
{
    return 1;
}

H_SAMPLE_RATE HWav::GetRate()
{
    return H_SAMPLE_RATE_48K;
}

size_t HWav::GetSize()
{
    return 0;
}

const char* HWav::GetFilename()
{
    return _filename;
}
