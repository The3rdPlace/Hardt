#ifndef __HNETWORKWRITER_CPP
#define __HNETWORKWRITER_CPP

#include "hnetworkwriter.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkWriter<T>::HNetworkWriter():
    _socket(-1)
{}

template <class T>
HNetworkWriter<T>::HNetworkWriter(int socket):
    _socket(socket)
{}

template <class T>
void HNetworkWriter<T>::SetSocket(int socket)
{
    _socket = socket;
}

template <class T>
int HNetworkWriter<T>::Write(T* src, size_t blocksize)
{
    this->Metrics.Writes++;
    int out = send(_socket, (void*) src, blocksize *  sizeof(T), 0 );
    if( out >= 0 )
    {
        this->Metrics.BlocksOut++;
        this->Metrics.BytesOut += out;

        return out / sizeof(T);
    }
    return out;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HNetworkWriter
template
HNetworkWriter<int8_t>::HNetworkWriter();

template
HNetworkWriter<uint8_t>::HNetworkWriter();

template
HNetworkWriter<int16_t>::HNetworkWriter();

template
HNetworkWriter<int32_t>::HNetworkWriter();

template
HNetworkWriter<int8_t>::HNetworkWriter(int socket);

template
HNetworkWriter<uint8_t>::HNetworkWriter(int socket);

template
HNetworkWriter<int16_t>::HNetworkWriter(int socket);

template
HNetworkWriter<int32_t>::HNetworkWriter(int socket);

// Write()
template
int HNetworkWriter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HNetworkWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HNetworkWriter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HNetworkWriter<int32_t>::Write(int32_t* src, size_t blocksize);

#endif