#ifndef __HNULLWRITER_CPP
#define __HNULLWRITER_CPP

#include "hnullwriter.h"

template <class T>
int HNullWriter<T>::Write(T* src, size_t blocksize)
{
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Write()
template
int HNullWriter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HNullWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HNullWriter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HNullWriter<int32_t>::Write(int32_t* src, size_t blocksize);

#endif