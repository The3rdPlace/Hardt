#ifndef __HWAVREADER_CPP
#define __HWAVREADER_CPP

#include "hwavreader.h"

template <class T>
HWavReader<T>::HWavReader(const char* filename):
    HWav(filename),
    HFileReader<T>(filename)
{}

template <class T>
bool HWavReader<T>::Start(void* unused)
{
    return HFileReader<T>::Start(unused) && HFileReader<T>::Read((T*) &_header, 44 / sizeof(T)) == 44 / sizeof(T);
}

/********************************************************************
Explicit instantiation
********************************************************************/

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
bool HWavReader<int8_t>::Start(void* unused);

template
bool HWavReader<uint8_t>::Start(void* unused);

template
bool HWavReader<int16_t>::Start(void* unused);

template
bool HWavReader<int32_t>::Start(void* unused);

#endif