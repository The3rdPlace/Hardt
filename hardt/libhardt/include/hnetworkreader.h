#ifndef __HNETWORKREADER_H
#define __HNETWORKREADER_H

#include <unistd.h>

/**
    Read from a network socket
*/
template <class T>
class HNetworkReader : public HReader<T>
{
    private:

        int _socket = -1;

    public:

        /** Construct a new HNetworkReader */
        HNetworkReader(std::string id);

        /** Construct a new HNetworkReader with an initialized socket */
        HNetworkReader(std::string id, int socket);

        /** Set the socket to use for reading */
        void SetSocket(int socket);

        /** Read a block of samples from the network */
        int ReadImpl(T* dest, size_t blocksize);

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // Command can go no further, transfering commands on the network is done by only by the HNetworkProcessor
            return true;
        }
};

#endif