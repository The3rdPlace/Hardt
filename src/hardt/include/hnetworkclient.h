#ifndef __HNETWORKCLIENT_H
#define __HNETWORKCLIENT_H

#include "hnetwork.h"

template <class T>
class HNetworkClient : public HNetwork<T>
{
    public:

        HNetworkClient(const char* address, int port, HWriter<T>* writer, bool* terminationToken);
        HNetworkClient(const char* address, int port, HReader<T>* reader, bool* terminationToken);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkClient<T>::HNetworkClient(const char* address, int port, HWriter<T>* writer, bool* terminationToken):
    HNetwork<T>(address, port, writer, terminationToken)
{
    HLog("HNetworkClient(address=%s, port=%d, address=%s writer=*, terminationToken=%d)", address, port, *terminationToken);
}

template <class T>
HNetworkClient<T>::HNetworkClient(const char* address, int port, HReader<T>* reader, bool* terminationToken):
    HNetwork<T>(address, port, reader, terminationToken)
{
    HLog("HNetworkClient(address=%s, port=%d, address=%s reader=*, terminationToken=%d)", address, port, *terminationToken);
}

#endif