#ifndef __HWRITER_CPP
#define __HWRITER_CPP

#include "hwriter.h"

template <class T>
bool HWriter<T>::Start(void* data)
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
bool HWriter<int8_t>::Start(void* data);

template
bool HWriter<uint8_t>::Start(void* data);

template
bool HWriter<int16_t>::Start(void* data);

template
bool HWriter<int32_t>::Start(void* data);

// Stop()
template
bool HWriter<int8_t>::Stop();

template
bool HWriter<uint8_t>::Stop();

template
bool HWriter<int16_t>::Stop();

template
bool HWriter<int32_t>::Stop();

#endif