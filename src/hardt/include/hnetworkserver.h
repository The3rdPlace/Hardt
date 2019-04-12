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

        void Setup();
        //void Teardown();



    public:

        HNetworkServer(int port, HWriter<T>* writer);
        HNetworkServer(int port, HReader<T>* reader);
        void Run();
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HWriter<T>* writer)
{
    this->_port = port;
    this->_serverSocket = -1;
    this->_clientSocket = -1;
    this->_connected = false;

    this->_writer = writer;
    this->_reader = NULL;

    Setup();
}

template <class T>
HNetworkServer<T>::HNetworkServer(int port, HReader<T>* reader)
{
    this->_port = port;
    this->_serverSocket = -1;
    this->_clientSocket = -1;
    this->_connected = false;

    this->_writer = NULL;
    this->_reader = reader;

    Setup();
}

//#define PORT 8080

template <class T>
void HNetworkServer<T>::Setup()
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

    // Forcefully attaching socket to the port 8080
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

    while(1)
    {
        std::cout << "LISTENING ON PORT " << _port << std::endl;

        if (listen(_serverSocket, 3) < 0)
        {
            //perror("listen");
            std::cout << "Error in listen" << std::endl;
            //throw new std::exception();
        }
        if ((_clientSocket = accept(_serverSocket, (struct sockaddr *)&_address,
                           (socklen_t*)&addrlen))<0)
        {
            //perror("accept");
            std::cout << "Error in accept" << std::endl;
            //return;
        }


        std::cout << "CONNECTED" << std::endl;
        int len;
        int shipped;

        while(1)
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
                    close(_clientSocket);
                    break;
                }
            }
        }

        std::cout << "CONNECTION CLOSED" << std::endl;
    }
    }
    catch( ... )
    {
        std::cout << "GLOBAL EXCEPTION" << std::endl;
    }
}

#endif