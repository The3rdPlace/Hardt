#ifndef __HNETWORKWRITER_CPP
#define __HNETWORKWRITER_CPP

#include "hnetworkwriter.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
int HNetworkWriter<T>::Write(T* src, size_t blocksize)
{
    this->Metrics.Writes++;
    int out = send(_socket, (void*) src, blocksize *  sizeof(T), 0 );
    if( out >= 0 )
    {
        this->Metrics.BlocksOut += out / sizeof(T);
        this->Metrics.BytesOut += out;

        return out / sizeof(T);
    }
    return out;
}

template <class T>
bool HNetworkWriter<T>::Start(void* socket)
{
    _socket = *((int*) socket);
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Write()
template
int HNetworkWriter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HNetworkWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HNetworkWriter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HNetworkWriter<int32_t>::Write(int32_t* src, size_t blocksize);

// Start()
template
bool HNetworkWriter<int8_t>::Start(void* socket);

template
bool HNetworkWriter<uint8_t>::Start(void* socket);

template
bool HNetworkWriter<int16_t>::Start(void* socket);

template
bool HNetworkWriter<int32_t>::Start(void* socket);

#endif