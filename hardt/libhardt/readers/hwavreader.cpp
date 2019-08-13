#ifndef __HWAVREADER_CPP
#define __HWAVREADER_CPP

#include "hwavreader.h"

template <class T>
HWavReader<T>::HWavReader(const char* filename):
    HWav(filename),
    HFileReader<T>(filename)
{}

template <class T>
bool HWavReader<T>::Start()
{
    return HFileReader<T>::Start() && HFileReader<T>::Read((T*) &_header, 44 / sizeof(T)) == 44 / sizeof(T);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HWavReader()
template
HWavReader<int8_t>::HWavReader(const char* filename);

template
HWavReader<uint8_t>::HWavReader(const char* filename);

template
HWavReader<int16_t>::HWavReader(const char* filename);

template
HWavReader<int32_t>::HWavReader(const char* filename);

// Start()
template
bool HWavReader<int8_t>::Start();

template
bool HWavReader<uint8_t>::Start();

template
bool HWavReader<int16_t>::Start();

template
bool HWavReader<int32_t>::Start();

//! @endcond
#endif