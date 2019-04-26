#ifndef __HNETWORKPROCESSOR_H
#define __HNETWORKPROCESSOR_H

#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>

template <class T>
class HNetworkProcessor : public HProcessor<T>
{
    private:

        int _port;
        int _serverSocket;
        int _clientSocket;
        struct sockaddr_in _address;
        const char* _server;
        bool* _terminated;
        T* _buffer;
        int _blocksize;

        HNetworkReader<T> _networkReader;
        HNetworkWriter<T> _networkWriter;

        bool _isServer;
        bool _isWriter;

        void InitServer();
        void InitClient();
        void RunServer();
        void RunClient();
        void RunProcessor();

    public:

        HNetworkProcessor(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetworkProcessor(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken);
        HNetworkProcessor(int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetworkProcessor(int port, HReader<T>* reader, int blocksize, bool* terminationToken);

        ~HNetworkProcessor();

        void Run();
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, &_networkReader),
    _isServer(false),
    _isWriter(true),
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize)
{
    HLog("HNetworkProcessor(client)(address=%s, port=%d, writer=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, _blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkWriter, reader),
    _isServer(false),
    _isWriter(false),
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize)
{
    HLog("HNetworkProcessor(client)(address=%s, port=%d, reader=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, _blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, &_networkReader),
    _isServer(true),
    _isWriter(true),
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize)
{
    HLog("HNetworkProcessor(server)(port=%d, writer=*, terminationToken=%d), blocksize is %d", port, *terminationToken, _blocksize);
    InitServer();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkWriter, reader),
    _isServer(true),
    _isWriter(false),
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize)
{
    HLog("HNetworkProcessor(server)(port=%d, reader=*, terminationToken=%d), blocksize is %d", port, *terminationToken,_blocksize);
    InitServer();
}

template <class T>
HNetworkProcessor<T>::~HNetworkProcessor()
{
    HLog("~HNetworkProcessor()");
    if( this->_clientSocket > -1 ) {
        HLog("Closing client socket");
        close(this->_clientSocket);
    }
    if( this->_serverSocket > -1 ) {
        HLog("Closing server socket");
        close(this->_serverSocket);
    }
    if( _buffer != NULL )
    {
        HLog("Releasing local buffer");
        delete _buffer;
    }
    HLog("Done");

}

template <class T>
void HNetworkProcessor<T>::InitServer()
{
    if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        HError("Failed to create server socket");
        throw new HInitializationException("Failed to create server socket");
    }
    HLog("Created server socket");

    // Forcefully attaching socket to the selected port
    int opt = 0;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        HError("setsockopt() failed");
        throw new HInitializationException("setsockopt() failed");
    }

    // Server address
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( _port );

    // Forcefully attaching socket to the selected port
    if (bind(_serverSocket, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        HError("bind() failed for port %d on INADDR_ANY", _port);
        throw new HInitializationException("bind() failed");
    }
    HLog("Bound to INADDR_ANY on port %d", _port);

    // Ignore the SIGPIPE signal since it occurres when ever a client closes the connection
    sigignore(SIGPIPE);
    HLog("SIGPIPE disabled");

    // Allocate local buffer
    _buffer = new T[_blocksize];
    HLog("Allocated buffer for %d frames = %d bytes", _blocksize, _blocksize * sizeof(T));
}

template <class T>
void HNetworkProcessor<T>::InitClient()
{
    // Create a socket
    if ((_clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        HError("Failed to create client socket");
        throw new HInitializationException("Failed to create client socket");
    }

    // Server address
    memset(&_address, '0', sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(_port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, _server, &_address.sin_addr)<=0)
    {
        HError("Invalid address %s", _server);
        throw new HNetworkException("Invalid address");
    }

    // Connect
    HLog("Trying to connect");
    if (connect(_clientSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        HError("Failed to connect to server %s", _server);
        throw new HNetworkException("Failed to connect to server");
    }
    HLog("Connected");

    // Allocate local buffer
    _buffer = new T[_blocksize];
    HLog("Allocated buffer for %d frames = %d bytes", _blocksize, _blocksize * sizeof(T));
}

template <class T>
void HNetworkProcessor<T>::Run()
{
    _server != NULL ? RunProcessor() : RunServer();
}

template <class T>
void HNetworkProcessor<T>::RunServer()
{
    try
    {
        int addrlen = sizeof(_address);

        // Run untill stopped
        while(!*_terminated)
        {
            // Wait for connection
            HLog("Listening on port %d", _port);
            if (listen(_serverSocket, 3) < 0)
            {
                HError("Error in listen(), socket may have been closed");
                HLog("Exit from Run() due to error");
                return;
            }

            // Accept new connection
            HLog("Waiting for connection");
            if ((_clientSocket = accept(_serverSocket, (struct sockaddr *)&_address, (socklen_t*)&addrlen))<0)
            {
                HError("Error in accept, socket may have been closed");
                HLog("Exit from Run() due to error");
                return;
            }
            HLog("Accepting new connection from %s", inet_ntoa(((struct sockaddr_in *) &_address)->sin_addr));

            // Handle readers or writers
            RunProcessor();

            // Close the socket, should it still be open
            if( _clientSocket > -1 )
            {
                HLog("Closing connection to the client");
                close(_clientSocket);
                _clientSocket = -1;
            }
            HLog("Connection closed");
            HLog(HProcessor<T>::_writer->GetMetrics("HNetworkProcessor::HProcessor::_writer").c_str());
            HProcessor<T>::_writer->ResetMetrics();
        }
    }
    catch( const std::exception& ex )
    {
        HError("Caught exception while handling client: %s", ex.what());
        if( _clientSocket > -1 )
        {
            HLog("Closing connection to the client");
            close(_clientSocket);
            _clientSocket = -1;
        }
        HLog("Exit from Run() due to exception");
        return;
    }
    HLog("Exit from Run()");

}

template <class T>
void HNetworkProcessor<T>::RunProcessor()
{
    // Start reader and writer - some readers/writers have start/stop handling
    HLog("Starting reader and writer, data is a socket");
    if( !HProcessor<T>::Start(&_clientSocket) )
    {
        HError("Failed to Start() reader or writer");
        return;
    }
    HLog("Reader and writer Start()'ed");

    // Read from reader and write to network
    HLog("Processing");
    while(!*_terminated)
    {
        // Read data from the reader
        int len;
        try
        {
            len = HProcessor<T>::Read(_buffer, _blocksize);
            if( len == 0 )
            {
                continue;
            }
            this->Metrics.Reads++;
            this->Metrics.BlocksIn += len;
            this->Metrics.BytesIn += len * sizeof(T);
        }
        catch( std::exception ex )
        {
            HError("Caught exception: %s", ex.what());
            break;
        }

        // Send the data to the connected client
        int shipped;
        try
        {
            this->Metrics.Writes++;
            shipped = HProcessor<T>::Write(_buffer, len);
            if( shipped <= 0 )
            {
                HLog("Zero write to the writer, stopping");
                break;
            }
            if( shipped != len )
            {
                HLog("Not all data was written, %d of %d ", shipped, len);
            }
            this->Metrics.BlocksOut += shipped;
            this->Metrics.BytesOut += shipped * sizeof(T);
        }
        catch( std::exception ex )
        {
            HError("Caught exception: %s", ex.what());
            break;
        }
    }

    // Stop the reader - some readers have start/stop handling
    HLog("Stopping reader and writer");
    if( HProcessor<T>::Stop() == false )
    {
        HError("Failed to Stop() reader or writer");
    }
    HLog("Reader and writer Stop()'ed");
}

#endif