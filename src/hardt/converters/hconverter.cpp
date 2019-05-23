#ifndef __HCONVERTER_CPP
#define __HCONVERTER_CPP

#include "hconverter.h"

template <class T>
HConverter<T>::HConverter(void (*readyCallback)())
{
    HLog("HConverter()");
}

template <class T>
void HConverter<T>::Ready()
{
    _readyCallback();
}

template <class T>
int HConverter<T>::Write(T* src, size_t blocksize)
{
    return Convert(src, blocksize);
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HConverter
template
HConverter<int8_t>::HConverter(void (*readyCallback)());

template
HConverter<uint8_t>::HConverter(void (*readyCallback)());

template
HConverter<int16_t>::HConverter(void (*readyCallback)());

template
HConverter<int32_t>::HConverter(void (*readyCallback)());

// Write()
template
int HConverter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HConverter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HConverter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HConverter<int32_t>::Write(int32_t* src, size_t blocksize);

// Ready()
template
void HConverter<int8_t>::Ready();

template
void HConverter<uint8_t>::Ready();

template
void HConverter<int16_t>::Ready();

template
void HConverter<int32_t>::Ready();

#endif