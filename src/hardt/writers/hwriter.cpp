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
bool HWriter<int>::Start(void* data);

// Stop()
template
bool HWriter<int>::Stop();

#endif