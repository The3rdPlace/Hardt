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
    int in = read( _socket, (void*) dest, blocksize * sizeof(T));
    if( in >= 0 )
    {
        return in / sizeof(T);
    }
    return in;
}

template <class T>
bool HNetworkReader<T>::Start(void* socket)
{
    _socket = *((int*) socket);
    return true;
}

#endif