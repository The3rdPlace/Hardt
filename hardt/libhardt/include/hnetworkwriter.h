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
        HNetworkWriter(std::string id);

        /** Construct a new HNetworkWriter with an initialized socket */
        HNetworkWriter(std::string id, int socket);

        /** Set the socket to use for writes */
        void SetSocket(int socket);

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // Command can go no further, transfering commands on the network is done by only by the HNetworkProcessor
            return true;
        }
};

#endif