#ifndef __HNETWORKPROCESSOR_H
#define __HNETWORKPROCESSOR_H

#include <netinet/in.h>

#include "hnetworkreader.h"
#include "hnetworkwriter.h"

/**
    A network enabled processor.

    The processor will either read from a network connection and write to a local writer,
    or read from a local reader and write to the a network connection.

    If you use the constructors _without_ the 'address' variable, the processor acts like
    the server and awaits connections, while using the constructor _with_ the address,
    the processor will attempt to connect to that address and either read from it or write to it.
*/
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

        /** Construct a new HNetworkProcessor that acts as a client, reading samples from the remote endpoint
            and writing them to a local writer. */
        HNetworkProcessor(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken);

        /** Construct a new HNetworkProcessor that acts as a client, reading samples from the remote endpoint
            and writing them to a local writer which must register by using the HWriterConsumer interface. */
        HNetworkProcessor(const char* address, int port, int blocksize, bool* terminationToken);

        /** Construct a new HNetworkProcessor that acts as a client, reading samples from a local reader and
            writing them to the remote endpoint */
        HNetworkProcessor(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken);

        /** Construct a new HNetworkProcessor that acts as a server, reading samples from the remote endpoint
            and writing them to a local writer. */
        HNetworkProcessor(int port, HWriter<T>* writer, int blocksize, bool* terminationToken);

        /** Construct a new HNetworkProcessor that acts as a server, reading samples from the remote endpoint
            and writing them to a local writer which must register by using the HWriterConsumer interface. */
        HNetworkProcessor(int port, int blocksize, bool* terminationToken);

        /** Construct a new HNetworkProcessor that acts as a server, reading samples from a local reader and
            writing them to the remote endpoint. */
        HNetworkProcessor(int port, HReader<T>* reader, int blocksize, bool* terminationToken);

        /** Default destructor */
        ~HNetworkProcessor();

        /** Run the processor */
        void Run(long unsigned int blocks = 0);

        /** Halt the processor. This will set the associated terminationToken to true */
        void Halt();
};

#endif