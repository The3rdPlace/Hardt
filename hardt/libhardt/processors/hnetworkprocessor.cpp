#ifndef __HNETWORKPROCESSOR_CPP
#define __HNETWORKPROCESSOR_CPP

#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "hnetworkprocessor.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, &_networkReader, blocksize, terminationToken),
    _isServer(false),
    _isWriter(true),
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(client)(address=%s, port=%d, writer=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int port, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkReader, blocksize, terminationToken),
    _isServer(false),
    _isWriter(true),
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(client)(address=%s, port=%d, terminationToken=%d), blocksize is %d", address, port, *terminationToken, blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkWriter, reader, blocksize, terminationToken),
    _isServer(false),
    _isWriter(false),
    _port(port),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(client)(address=%s, port=%d, reader=*, terminationToken=%d), blocksize is %d", address, port, *terminationToken, blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int port, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, &_networkReader, blocksize, terminationToken),
    _isServer(true),
    _isWriter(true),
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(server)(port=%d, writer=*, terminationToken=%d), blocksize is %d", port, *terminationToken, blocksize);
    InitServer();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int port, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkReader, blocksize, terminationToken),
    _isServer(true),
    _isWriter(true),
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(server)(port=%d, terminationToken=%d), blocksize is %d", port, *terminationToken, blocksize);
    InitServer();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int port, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkWriter, reader, blocksize, terminationToken),
    _isServer(true),
    _isWriter(false),
    _port(port),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(server)(port=%d, reader=*, terminationToken=%d), blocksize is %d", port, *terminationToken, blocksize);
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

    // Convert the server hostname (or address) to a true ip address
    HLog("Attempting lookup of server hostname: %s", _server);
    hostent* record = gethostbyname(_server);
	if(record == NULL)
	{
	    HError("Unknown hostname: %s", _address);
	    throw new HInitializationException("Unknown hostname");
	}
	in_addr* address = (in_addr*) record->h_addr;
	_server = inet_ntoa(*address);
    HLog("Server hostname = %s", _server);

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
    HLog("Trying to connect to %s:%d", _server, _port);
    if (connect(_clientSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        HError("Failed to connect to server %s", _server);
        throw new HNetworkException("Failed to connect to server");
    }
    HLog("Connected");
}

template <class T>
void HNetworkProcessor<T>::Run(long unsigned int blocks)
{
    _server != NULL ? RunClient(blocks) : RunServer(blocks);
    HLog(this->GetMetrics("HNetworkProcessor").c_str());
}

template <class T>
void HNetworkProcessor<T>::RunServer(long unsigned int blocks)
{
    // Prepare select()
    int activity;
    fd_set rfds;
    struct timeval tv;

    // Run server untill terminated
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

            //wait for activity (connect)
            while( !*_terminated )
            {
                // Wait for activity on the serversocket.
                // This is not the optimal way to check for exit, but it will suffice
                // for the types of scenarios that we will encounter, running on small
                // isolated boxes (raspberry's etc.) or just handling a very limited number
                // of connections. Some days we'll make this beautifull though.
                tv.tv_sec = 3;
                tv.tv_usec = 0;
                FD_ZERO(&rfds);
                FD_SET(_serverSocket, &rfds);
                activity = select( _serverSocket + 1 , &rfds , &rfds , NULL , &tv);
                if ( activity < 0)
                {
                    HLog("Error when waiting for connections");
                    return;
                }
                else if( activity > 0 )
                {
                    // Someone is trying to connect
                    break;
                }
            }

            // Check for termination
            if( *_terminated )
            {
                HLog("Termination token set to true, halting");
                break;
            }

            // Accept new connection
            HLog("Accepting connection");
            if ((_clientSocket = accept(_serverSocket, (struct sockaddr *)&_address, (socklen_t*)&addrlen))<0)
            {
                HError("Error in accept, socket may have been closed");
                HLog("Exit from Run() due to error");
                return;
            }
            HLog("Accepting new connection from %s", inet_ntoa(((struct sockaddr_in *) &_address)->sin_addr));

            // Handle readers or writers
            RunProcessor(blocks);

            // Close the socket, should it still be open
            if( _clientSocket > -1 )
            {
                HLog("Closing connection to the client");
                close(_clientSocket);
                _clientSocket = -1;
            }
            HLog("Connection closed");
            HLog(HProcessor<T>::GetWriter()->GetMetrics("HNetworkProcessor::HProcessor::_writer").c_str());
            HLog(HProcessor<T>::GetReader()->GetMetrics("HNetworkProcessor::HProcessor::_reader").c_str());
            HProcessor<T>::GetWriter()->ResetMetrics();
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
void HNetworkProcessor<T>::RunClient(long unsigned int blocks)
{
    RunProcessor(blocks);
    if( _clientSocket > -1 )
    {
        HLog("Closing connection to the client");
        close(_clientSocket);
        _clientSocket = -1;
    }
    HLog(HProcessor<T>::GetWriter()->GetMetrics("HNetworkProcessor::HProcessor::_writer").c_str());
    HLog(HProcessor<T>::GetReader()->GetMetrics("HNetworkProcessor::HProcessor::_reader").c_str());
}

template <class T>
void HNetworkProcessor<T>::RunProcessor(long unsigned int blocks)
{
    _networkReader.SetSocket(_clientSocket);
    _networkWriter.SetSocket(_clientSocket);
    HProcessor<T>::Run(blocks);
}

template <class T>
void HNetworkProcessor<T>::Halt()
{
    HLog("Halting processor");
    if( _clientSocket > -1 ) {
        HLog("Closing client socket");
        close(_clientSocket);
        _clientSocket = -1;
    }
    if( _serverSocket > -1 ) {
        HLog("Closing server socket");
        close(_serverSocket);
        _serverSocket = -1;
    }
    HProcessor<T>::Halt();
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HNetworkProcessor()
template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int port, HWriter<int8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int port, HWriter<uint8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int port, HWriter<int16_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int port, HWriter<int32_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int port, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int port, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int port, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int port, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(int port, HWriter<int8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int port, HWriter<uint8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(int port, HWriter<int16_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(int port, HWriter<int32_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(int port, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(int port, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int port, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(int port, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(int port, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

// ~HNetworkProcessor
template
HNetworkProcessor<int8_t>::~HNetworkProcessor();

template
HNetworkProcessor<uint8_t>::~HNetworkProcessor();

template
HNetworkProcessor<int16_t>::~HNetworkProcessor();

template
HNetworkProcessor<int32_t>::~HNetworkProcessor();

// Run()
template
void HNetworkProcessor<int8_t>::Run(long unsigned int blocks);

template
void HNetworkProcessor<uint8_t>::Run(long unsigned int blocks);

template
void HNetworkProcessor<int16_t>::Run(long unsigned int blocks);

template
void HNetworkProcessor<int32_t>::Run(long unsigned int blocks);

// Halt()
template
void HNetworkProcessor<int8_t>::Halt();

template
void HNetworkProcessor<uint8_t>::Halt();

template
void HNetworkProcessor<int16_t>::Halt();

template
void HNetworkProcessor<int32_t>::Halt();

//! @endcond
#endif