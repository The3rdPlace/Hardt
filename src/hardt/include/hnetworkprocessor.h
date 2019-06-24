#ifndef __HNETWORKPROCESSOR_H
#define __HNETWORKPROCESSOR_H

#include <netinet/in.h>

#include "hnetworkreader.h"
#include "hnetworkwriter.h"

template <class T>
class HNetworkProcessor : public HProcessor<T>
{
    private:

        int _port;
        int _serverSocket;
        int _clientSocket;
        struct sockaddr_in _address;
        const char* _server;
        HNetworkReader<T> _networkReader;
        HNetworkWriter<T> _networkWriter;
        bool* _terminated;

        bool _isServer;
        bool _isWriter;

        void InitServer();
        void InitClient();
        void RunServer(long unsigned int blocks);
        void RunClient(long unsigned int blocks);
        void RunProcessor(long unsigned int blocks);

    public:

        HNetworkProcessor(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetworkProcessor(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken);
        HNetworkProcessor(int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetworkProcessor(int port, HReader<T>* reader, int blocksize, bool* terminationToken);

        ~HNetworkProcessor();

        void Run(long unsigned int blocks = 0);
        void Halt();
};

#endif