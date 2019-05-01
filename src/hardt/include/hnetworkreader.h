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

#endif