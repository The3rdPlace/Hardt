#ifndef __HNETWORK_H
#define __HNETWORK_H

#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>

template <class T>
class HNetwork
{
    private:

        int _port;
        int _serverSocket;
        int _clientSocket;
        bool _connected;
        HWriter<T>* _writer;
        HReader<T>* _reader;
        struct sockaddr_in _address;
        const char* _server;
        bool* _terminated;
        bool _firstRead;
        bool _firstWrite;
        T* _buffer;
        int _blocksize;
        HReader<T>* _nwReader;
        HWriter<T>* _nwWriter;
        bool _deallocNwStreams;

        void InitServer();
        void InitClient();
        void RunReader(T* buffer);
        void RunWriter(T* buffer);

    protected:

        HNetwork(const char* address, int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken);
        HNetwork(const char* address, int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken);
        HNetwork(int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken);
        HNetwork(int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken);

        HNetwork(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetwork(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken);
        HNetwork(int port, HWriter<T>* writer, int blocksize, bool* terminationToken);
        HNetwork(int port, HReader<T>* reader, int blocksize, bool* terminationToken);

        ~HNetwork();

    public:

        void Run();
        void RunServer();
        void RunClient();
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetwork<T>::HNetwork(const char* address, int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken):
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(writer),
    _reader(NULL),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _nwReader(networkReader),
    _deallocNwStreams(false)
{
    HLog("HNetwork(client)(address=%s, port=%d, writer=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, _blocksize);
    InitClient();
}

template <class T>
HNetwork<T>::HNetwork(const char* address, int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken):
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(NULL),
    _reader(reader),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _nwWriter(networkWriter),
    _deallocNwStreams(false)
{
    HLog("HNetwork(client)(address=%s, port=%d, reader=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, _blocksize);
    InitClient();
}

template <class T>
HNetwork<T>::HNetwork(int port, HWriter<T>* writer, int blocksize, HReader<T>* networkReader, bool* terminationToken):
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(writer),
    _reader(NULL),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _nwReader(networkReader),
    _deallocNwStreams(false)
{
    HLog("HNetwork(server)(port=%d, writer=*, terminationToken=%d), blocksize is %d", port, *terminationToken, _blocksize);
    InitServer();
}

template <class T>
HNetwork<T>::HNetwork(int port, HReader<T>* reader, int blocksize, HWriter<T>* networkWriter, bool* terminationToken):
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(NULL),
    _reader(reader),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _nwWriter(networkWriter),
    _deallocNwStreams(false)
{
    HLog("HNetwork(server)(port=%d, reader=*, terminationToken=%d), blocksize is %d", port, *terminationToken,_blocksize);
    InitServer();
}

template <class T>
HNetwork<T>::HNetwork(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(writer),
    _reader(NULL),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _deallocNwStreams(true)
{
    HLog("HNetwork(client)(address=%s, port=%d, writer=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, _blocksize);
    _nwReader = new HNetworkReader<T>();
    _nwWriter = NULL;
    InitClient();
}

template <class T>
HNetwork<T>::HNetwork(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(NULL),
    _reader(reader),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _deallocNwStreams(true)
{
    HLog("HNetwork(client)(address=%s, port=%d, reader=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, _blocksize);
    _nwWriter = new HNetworkWriter<T>();
    _nwReader = NULL;
    InitClient();
}

template <class T>
HNetwork<T>::HNetwork(int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(writer),
    _reader(NULL),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _deallocNwStreams(true)
{
    HLog("HNetwork(server)(port=%d, writer=*, terminationToken=%d), blocksize is %d", port, *terminationToken, _blocksize);
    _nwReader = new HNetworkReader<T>();
    _nwWriter = NULL;
    InitServer();
}

template <class T>
HNetwork<T>::HNetwork(int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _connected(false),
    _writer(NULL),
    _reader(reader),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize),
    _deallocNwStreams(true)
{
    HLog("HNetwork(server)(port=%d, reader=*, terminationToken=%d), blocksize is %d", port, *terminationToken,_blocksize);
    _nwWriter = new HNetworkWriter<T>();
    _nwReader = NULL;
    InitServer();
}

template <class T>
HNetwork<T>::~HNetwork()
{
    HLog("~HNetwork()");
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
    if( _nwReader != NULL && _deallocNwStreams )
    {
        HLog("Releasing local network reader");
        delete (HNetworkReader<T>*) _nwReader;
    }
    if( _nwWriter != NULL && _deallocNwStreams )
    {
        HLog("Releasing local network writer");
        delete (HNetworkWriter<T>*) _nwWriter;
    }
    HLog("Done");

}

template <class T>
void HNetwork<T>::InitServer()
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
void HNetwork<T>::InitClient()
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
    HLog("Allocate");
    HLog("Allocate %d", _blocksize);
    _buffer = new T[_blocksize];
    HLog("Allocated buffer for %d frames = %d bytes", _blocksize, _blocksize * sizeof(T));
}

template <class T>
void HNetwork<T>::Run()
{
    _server != NULL ? RunClient() : RunServer();
}

template <class T>
void HNetwork<T>::RunServer()
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
            HLog("Accepting new connection from %s", inet_ntoa(((struct sockaddr_in *) &_address)->sin_addr));
            if ((_clientSocket = accept(_serverSocket, (struct sockaddr *)&_address, (socklen_t*)&addrlen))<0)
            {
                HError("Error in accept, socket may have been closed");
                HLog("Exit from Run() due to error");
                return;
            }

            // Handle readers or writers
            if( _reader != NULL )
            {
                RunReader(_buffer);
            }
            else if( _writer != NULL )
            {
                RunWriter(_buffer);
            }
            else
            {
                HError("Neither a reader or a writer has been given!!  Disconnection the client");
            }

            // Close the socket, should it still be open
            if( _clientSocket > -1 )
            {
                HLog("Closing connection to the client");
                close(_clientSocket);
                _clientSocket = -1;
            }
            HLog("Connection closed");
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
void HNetwork<T>::RunClient()
{
    // Handle readers or writers
    if( _reader != NULL )
    {
        RunReader(_buffer);
    }
    else if( _writer != NULL )
    {
        RunWriter(_buffer);
    }
    else
    {
        HError("Neither a reader or a writer has been given!!  Disconnection the client");
    }
}

template <class T>
void HNetwork<T>::RunReader(T* buffer)
{
    HLog("Reading samples from the reader and putting them on the network");
    _firstRead = true;

    // Start reader and writer - some readers/writers have start/stop handling
    HLog("Starting reader");
    if( !_reader->Start(NULL) )
    {
        HError("Failed to Start() reader");
        return;
    }
    HLog("Reader Start()'ed");
    HLog("Starting writer");
    if( !_nwWriter->Start(&_clientSocket) )
    {
        HError("Failed to Start() writer");
        _reader->Stop();
        return;
    }
    HLog("Writer Start()'ed");

    // Read from reader and write to network
    while(!*_terminated)
    {
        // Read data from the reader
        int len;
        try
        {
            len = _reader->Read(buffer, _blocksize);
            if( len <= 0 )
            {
                HLog("Zero read from the reader, possibly unclean termination");
                break;
            }

            // Log only first frame to verify that the function gets called
            if( _firstRead )
            {
                HLog("Read %d frames from the reader", len);
            }
        }
        catch( std::exception ex )
        {
            HError("Caught exception: %s", ex.what());
            break;
        }

        // Send the data to the connected client
        if( len > 0 )
        {
            int shipped;
            try
            {
                shipped = _nwWriter->Write(buffer, len );
                if( shipped <= 0 )
                {
                    HLog("Zero write to the server, possibly closed socket");
                    break;
                }

                // Log only first frame to verify that the function writes to the network client
                if( _firstRead )
                {
                    HLog("Wrote %d bytes to the server", shipped);
                    _firstRead = false;
                }
            }
            catch( std::exception ex )
            {
                HError("Caught exception: %s", ex.what());
                break;
            }
        }
    }

    // Stop the reader - some readers have start/stop handling, others
    // just ignore this call
    HLog("Stopping reader");
    if( !_reader->Stop() )
    {
        HError("Failed to Stop() reader");
    }
    HLog("Reader Stop()'ed");
    HLog("Stopping writer");
    if( !_nwWriter->Stop() )
    {
        HError("Failed to Stop() writer");
    }
    HLog("Writer stopped");
}

template <class T>
void HNetwork<T>::RunWriter(T* buffer)
{
    HLog("Reading samples from the network and writing them to the writer");
    _firstWrite = true;

    // Start readers and writers - some readers/writers have start/stop handling
    HLog("Starting writer");
    if( !_writer->Start(NULL) )
    {
        HError("Failed to Start() writer");
        return;
    }
    HLog("Writer Start()'ed");
    HLog("Starting reader");
    if( !_nwReader->Start(&_clientSocket) )
    {
        HError("Failed to Start() reader");
        _writer->Stop();
        return;
    }
    HLog("Reader Start()'ed");

    // Read from the network and write to the writer
    while(!*_terminated)
    {
        // Read data from the connected server
        int received;
        try
        {
            received = _nwReader->Read(buffer, _blocksize);
            if( received <= 0 )
            {
                HLog("Zero read from the server, possibly closed socket");
                break;
            }

            // Log only first frame to verify that the function writes to the network client
            if( _firstWrite )
            {
                HLog("Read %d bytes from the server", received);
            }
        }
        catch( std::exception ex )
        {
            HError("Caught exception: %s", ex.what());
            break;
        }

        // Write data to the writer
        if( received > 0)
        {
            int len;
            try
            {
                len = _writer->Write(buffer, _blocksize);
                if( len <= 0 )
                {
                    HLog("Zero write to the reader, possibly unclean termination");
                    break;
                }

                // Log only first frame to verify that the function gets called
                if( _firstWrite )
                {
                    HLog("Wrote %d frames from to the writer", len);
                    _firstWrite = false;
                }
            }
            catch( std::exception ex )
            {
                HError("Caught exception: %s", ex.what());
                break;
            }
        }
    }

    // Stop the writer - some writer have start/stop handling, others
    // just ignore this call
    HLog("Stopping writer");
    if( !_writer->Stop() )
    {
        HError("Failed to Stop() writer");
    }
    HLog("Writer Stop()'ed");
    HLog("Stopping reader");
    if( !_nwReader->Stop() )
    {
        HError("Failed to Stop() reader");
    }
    HLog("Reader stopped");
}

#endif