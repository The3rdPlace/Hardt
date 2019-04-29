#ifndef __HNETWORKREADER_H
#define __HNETWORKREADER_H

#include <unistd.h>

template <class T>
class HNetworkReader : public HReader<T>
{
    private:

        int _socket;

    public:

        int Read(T* dest, size_t blocksize);
        bool Start(void* socket);
};

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

#endif