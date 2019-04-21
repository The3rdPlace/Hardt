#ifndef __HNETWORKCLIENT_H
#define __HNETWORKCLIENT_H

template <class T>
class HNetworkClient : public HNetwork<T>
{
    public:

        HNetworkClient(const char* address, int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken);
        HNetworkClient(const char* address, int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken);

        HNetworkClient(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetworkClient(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkClient<T>::HNetworkClient(const char* address, int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken):
    HNetwork<T>(address, port, writer, blocksize, networkReader, terminationToken)
{
    HLog("HNetworkClient(address=%s, port=%d, address=%s writer=*, terminationToken=%d)", address, port, *terminationToken);
}

template <class T>
HNetworkClient<T>::HNetworkClient(const char* address, int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken):
    HNetwork<T>(address, port, reader, blocksize, networkWriter, terminationToken)
{
    HLog("HNetworkClient(address=%s, port=%d, address=%s reader=*, terminationToken=%d)", address, port, *terminationToken);
}

template <class T>
HNetworkClient<T>::HNetworkClient(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HNetwork<T>(address, port, writer, blocksize, terminationToken)
{
    HLog("HNetworkClient(address=%s, port=%d, address=%s writer=*, terminationToken=%d)", address, port, *terminationToken);
}

template <class T>
HNetworkClient<T>::HNetworkClient(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    HNetwork<T>(address, port, reader, blocksize, terminationToken)
{
    HLog("HNetworkClient(address=%s, port=%d, address=%s reader=*, terminationToken=%d)", address, port, *terminationToken);
}

#endif