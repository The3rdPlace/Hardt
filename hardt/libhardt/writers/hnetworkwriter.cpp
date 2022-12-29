#ifndef __HNETWORKWRITER_CPP
#define __HNETWORKWRITER_CPP

#include "hnetworkwriter.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkWriter<T>::HNetworkWriter(std::string id):
    HWriter<T>(id),
    _socket(-1)
{}

template <class T>
HNetworkWriter<T>::HNetworkWriter(std::string id, int socket):
    HWriter<T>(id),
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
    int out = send(_socket, (void*) src, blocksize *  sizeof(T), 0 );
    if( out >= 0 )
    {
        return out / sizeof(T);
    }
    return out;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HNetworkWriter
template
HNetworkWriter<int8_t>::HNetworkWriter(std::string id);

template
HNetworkWriter<uint8_t>::HNetworkWriter(std::string id);

template
HNetworkWriter<int16_t>::HNetworkWriter(std::string id);

template
HNetworkWriter<int32_t>::HNetworkWriter(std::string id);

template
HNetworkWriter<int8_t>::HNetworkWriter(std::string id, int socket);

template
HNetworkWriter<uint8_t>::HNetworkWriter(std::string id, int socket);

template
HNetworkWriter<int16_t>::HNetworkWriter(std::string id, int socket);

template
HNetworkWriter<int32_t>::HNetworkWriter(std::string id, int socket);

// Write()
template
int HNetworkWriter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HNetworkWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HNetworkWriter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HNetworkWriter<int32_t>::Write(int32_t* src, size_t blocksize);

//! @endcond
#endif