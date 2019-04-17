#ifndef __HNETWORKSERVER_H
#define __HNETWORKSERVER_H

#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

template <class T>
class HNetworkServer
{
    private:

        int _port;
        int _serverSocket;
        int _clientSocket;
        bool _connected;
        HWriter<T>* _writer;
        HReader<T>* _reader;
        struct sockaddr_in _address;
        bool* _terminated;
        bool _firstRead;
        bool _firstWrite;

        void Init();
        void RunReader(T* buffer);
        void RunWriter(T* buffer);

    public:

        HNetworkServer(int port, HWriter<T>* writer, bool* terminationToken);
        HNetworkServer(int port, HReader<T>* reader, bool* terminationToken);
        ~HNetworkServer();
        void Run();
        void Halt();
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HWriter<T>* writer, bool* terminationToken):
    _port(port),
    _serverSocket(-1),
    _clientSocket(-1),
    _connected(false),
    _writer(writer),
    _reader(NULL),
    _terminated(terminationToken)
{
    HLog("HNetworkServer(port=%d, writer=*, terminationToken=%d)", port, *terminationToken);
    Init();
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HReader<T>* reader, bool* terminationToken):
    _port(port),
    _serverSocket(-1),
    _clientSocket(-1),
    _connected(false),
    _writer(NULL),
    _reader(reader),
    _terminated(terminationToken)
{
    HLog("HNetworkServer(port=%d, reader=*, terminationToken=%d)", port, *terminationToken);
    Init();
}

template <class T>
HNetworkServer<T>::~HNetworkServer()
{
    HLog("~HNetworkServer()");
    if( this->_serverSocket > -1 ) {
        HLog("Closing server socket");
        close(this->_serverSocket);
    }
    if( this->_clientSocket > -1 ) {
        HLog("Closing client socket");
        close(this->_clientSocket);
    }
}

template <class T>
void HNetworkServer<T>::Init()
{
    int valread;
    int opt = 0;

    if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        HError("Failed to create server socket");
        throw new HInitializationException("Failed to create server socket");
    }
    HLog("Created server socket");

    // Forcefully attaching socket to the port 8080
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        HError("setsockopt() failed");
        throw new HInitializationException("setsockopt() failed");
    }

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
}

template <class T>
void HNetworkServer<T>::Run()
{
    try
    {
        int addrlen = sizeof(_address);
        T* buffer;

        // Allocate local buffer
        buffer = new T[_reader->Blocksize()];
        HLog("Allocated buffer for %d frames = %d bytes", _reader->Blocksize(), _reader->Blocksize() * sizeof(T));

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
                RunReader(buffer);
            }
            else if( _writer != NULL )
            {
                RunWriter(buffer);
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
void HNetworkServer<T>::RunReader(T* buffer)
{
    HLog("Connected to client, reading samples from the reader and sending them to the client");
    _firstRead = true;

    // Start the reader - some readers have start/stop handling, others
    // just ignore this call
    if( !_reader->Start() )
    {
        HError("Failed to Start() reader");
    }

    // Read from reader and write to network
    while(!*_terminated)
    {
        // Read data from the reader
        int len;
        try
        {
            len = _reader->Read(buffer);
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
                shipped = send(_clientSocket, (void*) buffer, len *  sizeof(T), 0 );
                if( shipped <= 0 )
                {
                    HLog("Zero write to the client, possibly closed socket");
                    break;
                }

                // Log only first frame to verify that the function writes to the network client
                if( _firstRead )
                {
                    HLog("Wrote %d bytes to the client", shipped);
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
    if( !_reader->Stop() )
    {
        HError("Failed to Stop() reader");
    }
}

template <class T>
void HNetworkServer<T>::RunWriter(T* buffer)
{
    HLog("Connected to client, reading samples from the client and writing them to the writer");
    throw new HNotImplementedException();
}

#endif