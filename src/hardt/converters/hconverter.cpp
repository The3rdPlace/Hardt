#ifndef __HCONVERTER_CPP
#define __HCONVERTER_CPP

#include "hconverter.h"

template <class T>
int HConverter<T>::Write(T* src, size_t blocksize)
{
    return Convert(src, blocksize);
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Write()
template
int HConverter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HConverter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HConverter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HConverter<int32_t>::Write(int32_t* src, size_t blocksize);

#endif