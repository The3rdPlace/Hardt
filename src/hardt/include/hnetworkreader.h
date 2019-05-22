#ifndef __HNETWORKREADER_H
#define __HNETWORKREADER_H

#include <unistd.h>

template <class T>
class HNetworkReader : public HReader<T>
{
    private:

        int _socket;

    public:

        HNetworkReader();
        HNetworkReader(int socket);

        void SetSocket(int socket);

        int Read(T* dest, size_t blocksize);
};

#endif