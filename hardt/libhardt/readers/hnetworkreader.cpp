#ifndef __HNETWORKREADER_CPP
#define __HNETWORKREADER_CPP

#include "hnetworkreader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkReader<T>::HNetworkReader(std::string id):
    HReader<T>(id),
    _socket(-1)
{}

template <class T>
HNetworkReader<T>::HNetworkReader(std::string id, int socket):
    HReader<T>(id),
    _socket(socket)
{}

template <class T>
void HNetworkReader<T>::SetSocket(int socket)
{
    _socket = socket;
}

template <class T>
int HNetworkReader<T>::ReadImpl(T* dest, size_t blocksize)
{
    // Read data
    int total = 0;
    int len = 0;
    while( total < blocksize * sizeof(T) )
    {
        int in = read( _socket, (void*) &dest[total / sizeof(T)], (blocksize * sizeof(T)) - total);
        if( in <= 0 )
        {
            HLog("Zero read from socket, socket may have been closed");
            return 0;
        }
        total += in;
    }

    return total / sizeof(T);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HNetworkReader
template
HNetworkReader<int8_t>::HNetworkReader(std::string id);

template
HNetworkReader<uint8_t>::HNetworkReader(std::string id);

template
HNetworkReader<int16_t>::HNetworkReader(std::string id);

template
HNetworkReader<int32_t>::HNetworkReader(std::string id);

template
HNetworkReader<int8_t>::HNetworkReader(std::string id, int socket);

template
HNetworkReader<uint8_t>::HNetworkReader(std::string id, int socket);

template
HNetworkReader<int16_t>::HNetworkReader(std::string id, int socket);

template
HNetworkReader<int32_t>::HNetworkReader(std::string id, int socket);

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
int HNetworkReader<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HNetworkReader<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HNetworkReader<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HNetworkReader<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif