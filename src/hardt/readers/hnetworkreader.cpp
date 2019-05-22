#ifndef __HNETWORKREADER_CPP
#define __HNETWORKREADER_CPP

#include "hnetworkreader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkReader<T>::HNetworkReader():
    _socket(-1)
{}

template <class T>
HNetworkReader<T>::HNetworkReader(int socket):
    _socket(socket)
{}

template <class T>
void HNetworkReader<T>::SetSocket(int socket)
{
    _socket = socket;
}

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

/********************************************************************
Explicit instantiation
********************************************************************/

// HNetworkReader
template
HNetworkReader<int8_t>::HNetworkReader();

template
HNetworkReader<uint8_t>::HNetworkReader();

template
HNetworkReader<int16_t>::HNetworkReader();

template
HNetworkReader<int32_t>::HNetworkReader();

template
HNetworkReader<int8_t>::HNetworkReader(int socket);

template
HNetworkReader<uint8_t>::HNetworkReader(int socket);

template
HNetworkReader<int16_t>::HNetworkReader(int socket);

template
HNetworkReader<int32_t>::HNetworkReader(int socket);

// SetSocket()
template
void HNetworkReader<int8_t>::SetSocket(int socket);

template
void HNetworkReader<uint8_t>::SetSocket(int socket);

template
void HNetworkReader<int16_t>::SetSocket(int socket);

template
void HNetworkReader<int32_t>::SetSocket(int socket);

// Read()
template
int HNetworkReader<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HNetworkReader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HNetworkReader<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HNetworkReader<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif