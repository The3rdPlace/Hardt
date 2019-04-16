#ifndef __HNETWORKSERVER_H
#define __HNETWORKSERVER_H

#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>

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

        void Init();

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
HNetworkServer<T>::HNetworkServer(int port, HWriter<T>* writer, bool* terminationToken) :
    _port(port),
    _serverSocket(-1),
    _clientSocket(-1),
    _connected(false),
    _writer(writer),
    _reader(NULL),
    _terminated(terminationToken)
{
    Init();
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HReader<T>* reader, bool* terminationToken) :
    _port(port),
    _serverSocket(-1),
    _clientSocket(-1),
    _connected(false),
    _writer(NULL),
    _reader(reader),
    _terminated(terminationToken)
{
    Init();
}

template <class T>
HNetworkServer<T>::~HNetworkServer()
{
    std::cout << "DESTRUCTING HNetworkServer" << std::endl;
    if( this->_serverSocket > -1 ) {
        close(this->_serverSocket);
    }
    if( this->_clientSocket > -1 ) {
        close(this->_clientSocket);
    }

    std::cout << "DONE" << std::endl;
}

template <class T>
void HNetworkServer<T>::Init()
{
    int valread;
    int opt = 0;


    if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        throw new std::exception();
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        std::cout << "setsockopt failed" << std::endl;
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( this->_port );

    // Forcefully attaching socket to the selected port
    if (bind(_serverSocket, (struct sockaddr *)&_address,
                                 sizeof(_address))<0)
    {
        perror("bind failed");
        throw new std::exception();
    }

    // Ignore the SIGPIPE signal since it occurres when ever a client closes the connection
    sigignore(SIGPIPE);
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
        std::cout << "allocated " << _reader->Blocksize() << " bytes" << std::endl;

        // Run untill stopped
        while(!*_terminated)
        {
            std::cout << "LISTENING ON PORT " << _port << std::endl;

            if (listen(_serverSocket, 3) < 0)
            {
                std::cout << "Error in listen" << std::endl;
                return;
            }
            if ((_clientSocket = accept(_serverSocket, (struct sockaddr *)&_address,
                               (socklen_t*)&addrlen))<0)
            {
                std::cout << "Error in accept" << std::endl;
                return;
            }


            std::cout << "CONNECTED" << std::endl;
            int len;
            int shipped;

            // Start the reader - some readers have start/stop handling, others
            // just ignore this call
            if( !_reader->Start() )
            {
                HError("Failed to Start() reader");
            }

            // Read from reader and write to network
            while(!*_terminated)
            {
                // Transfer frames to network
                try
                {
                    std::cout << "read" << std::endl;
                    len = _reader->Read(buffer);
                    std::cout << "got " << len << " bytes" << std::endl;
                    if( len <= 0 )
                    {
                        std::cout << "ZERO READ FROM CLIENT " << len << std::endl;
                        break;
                    }
                }
                catch( ... )
                {
                    std::cout << "Exception in Read()" << std::endl;
                    break;
                }
                if( len > 0 )
                {
                    try
                    {
                        std::cout << "try send" << std::endl;
                        shipped = send(_clientSocket, (void*) buffer, len *  sizeof(T), 0 );
                        std::cout << "shipped " << shipped << std::endl;
                        if( shipped <= 0 )
                        {
                            std::cout << "ZERO WRITE TO CLIENT " << shipped << std::endl;
                            break;
                        }
                    }
                    catch( ... )
                    {
                        std::cout << "Caught exception... stopping" << std::endl;
                        //s_interrupted = 1;
                        //return;

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

            // Close the socket, should it still be open
            if( _clientSocket > -1 )
            {
                close(_clientSocket);
                _clientSocket = -1;
            }
            std::cout << "CONNECTION CLOSED" << std::endl;
        }
    }
    catch( const std::exception& ex )
    {
        std::cout << "GLOBAL EXCEPTION" << std::endl;
        std::cout << ex.what() << std::endl;
        if( _clientSocket > -1 )
        {
            close(_clientSocket);
            _clientSocket = -1;
        }
        return;
    }
}

#endif