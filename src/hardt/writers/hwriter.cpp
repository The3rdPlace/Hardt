#ifndef __HWRITER_CPP
#define __HWRITER_CPP

#include "hwriter.h"

template <class T>
bool HWriter<T>::Start()
{
    return true;
}

template <class T>
bool HWriter<T>::Stop()
{
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Start()
template
bool HWriter<int8_t>::Start();

template
bool HWriter<uint8_t>::Start();

template
bool HWriter<int16_t>::Start();

template
bool HWriter<int32_t>::Start();

template
bool HWriter<long>::Start();

// Stop()
template
bool HWriter<int8_t>::Stop();

template
bool HWriter<uint8_t>::Stop();

template
bool HWriter<int16_t>::Stop();

template
bool HWriter<int32_t>::Stop();

template
bool HWriter<long>::Stop();

#endif