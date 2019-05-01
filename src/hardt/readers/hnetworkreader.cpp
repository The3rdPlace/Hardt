#ifndef __HNETWORKREADER_CPP
#define __HNETWORKREADER_CPP

#include "hnetworkreader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
int HNetworkReader<T>::Read(T* dest, size_t blocksize)
{
    // Read data
    int in = read( _socket, (void*) dest, blocksize * sizeof(T));
    if( in <= 0 )
    {
        HLog("Zero read from socket, socket may have been closed");
        return 0;
    }

    this->Metrics.Reads++;
    this->Metrics.BlocksIn += in / sizeof(T);
    this->Metrics.BytesIn += in;

    return in / sizeof(T);
}

template <class T>
bool HNetworkReader<T>::Start(void* socket)
{
    _socket = *((int*) socket);
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Read()
template
int HNetworkReader<int>::Read(int* dest, size_t blocksize);

// Start()
template
bool HNetworkReader<int>::Start(void* socket);

#endif