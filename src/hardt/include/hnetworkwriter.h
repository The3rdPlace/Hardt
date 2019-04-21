#ifndef __HNETWORKWRITER_H
#define __HNETWORKWRITER_H

#include <sys/socket.h>

template <class T>
class HNetworkWriter : public HWriter<T>
{
    private:

        int _socket;

    public:

        int Write(T* src, size_t blocksize);
        bool Start(void* socket);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
int HNetworkWriter<T>::Write(T* src, size_t blocksize)
{
    return send(_socket, src, blocksize *  sizeof(T), 0 );
}

template <class T>
bool HNetworkWriter<T>::Start(void* socket)
{
    _socket = *((int*) socket);
    return true;
}

#endif