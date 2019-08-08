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

        /** Construct a new HNetworkWriter */
        HNetworkWriter();

        /** Construct a new HNetworkWriter with an initialized socket */
        HNetworkWriter(int socket);

        /** Set the socket to use for writes */
        void SetSocket(int socket);

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);
};

#endif