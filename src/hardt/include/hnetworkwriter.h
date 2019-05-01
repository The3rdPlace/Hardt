#ifndef __HNETWORKWRITER_H
#define __HNETWORKWRITER_H

#include <sys/socket.h>

#include "hwriter.h"

template <class T>
class HNetworkWriter : public HWriter<T>
{
    private:

        int _socket;

    public:

        int Write(T* src, size_t blocksize);
        bool Start(void* socket);
};

#endif