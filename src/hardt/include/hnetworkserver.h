#ifndef __HNETWORKSERVER_H
#define __HNETWORKSERVER_H

#include "hnetwork.h"

template <class T>
class HNetworkServer : public HNetwork<T>
{
    public:

        HNetworkServer(int port, HWriter<T>* writer, bool* terminationToken);
        HNetworkServer(int port, HReader<T>* reader, bool* terminationToken);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HWriter<T>* writer, bool* terminationToken):
    HNetwork<T>(port, writer, terminationToken)
{
    HLog("HNetworkServer(port=%d, writer=*, terminationToken=%d)", port, *terminationToken);
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HReader<T>* reader, bool* terminationToken):
    HNetwork<T>(port, reader, terminationToken)
{
    HLog("HNetworkServer(port=%d, reader=*, terminationToken=%d)", port, *terminationToken);
}

#endif