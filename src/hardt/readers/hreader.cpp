#ifndef __HREADER_CPP
#define __HREADER_CPP

#include "hreader.h"

template <class T>
bool HReader<T>::Start()
{
    return true;
}

template <class T>
bool HReader<T>::Stop()
{
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Start()
template
bool HReader<int8_t>::Start();

template
bool HReader<uint8_t>::Start();

template
bool HReader<int16_t>::Start();

template
bool HReader<int32_t>::Start();

// Stop()
template
bool HReader<int8_t>::Stop();

template
bool HReader<uint8_t>::Stop();

template
bool HReader<int16_t>::Stop();

template
bool HReader<int32_t>::Stop();

#endif