#ifndef __HNETWORKWRITER_H
#define __HNETWORKWRITER_H

#include <sys/socket.h>

#include "hwriter.h"

/**
    Write to a network socket
*/
template <class T>
class HNetworkWriter : public HWriter<T>
{
    private:

        int _socket;

    public:

        HNetworkWriter();
        HNetworkWriter(int socket);

        void SetSocket(int socket);

        int Write(T* src, size_t blocksize);
};

#endif