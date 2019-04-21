#ifndef __HNETWORKSERVER_H
#define __HNETWORKSERVER_H

template <class T>
class HNetworkServer : public HNetwork<T>
{
    public:

        HNetworkServer(int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken);
        HNetworkServer(int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken);

        HNetworkServer(int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetworkServer(int port, HReader<T>* reader, int blocksize, bool* terminationToken);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken):
    HNetwork<T>(port, writer, blocksize, networkReader, terminationToken)
{
    HLog("HNetworkServer(port=%d, writer=*, terminationToken=%d)", port, blocksize, *terminationToken);
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken):
    HNetwork<T>(port, reader, blocksize, networkWriter, terminationToken)
{
    HLog("HNetworkServer(port=%d, reader=*, terminationToken=%d)", port, blocksize, *terminationToken);
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HNetwork<T>(port, writer, blocksize, terminationToken)
{
    HLog("HNetworkServer(port=%d, writer=*, terminationToken=%d)", port, blocksize, *terminationToken);
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    HNetwork<T>(port, reader, blocksize, terminationToken)
{
    HLog("HNetworkServer(port=%d, reader=*, terminationToken=%d)", port, blocksize, *terminationToken);
}

#endif