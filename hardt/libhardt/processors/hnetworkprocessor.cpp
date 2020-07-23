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
#include <thread>
#include <chrono>

#include "hnetworkprocessor.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, &_networkReader, blocksize, terminationToken),
    _isServer(false),
    _isWriter(true),
    _reader(nullptr),
    _writer(writer),
    _dataPort(dataPort),
    _commandPort(commandPort + 1),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _commandSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(client)(address=%s, dataPort=%d, commandPort=%d, writer=*, terminationToken=%d), blocksize is %d", address, dataPort, commandPort, *terminationToken, blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int dataPort, int commandPort, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkReader, blocksize, terminationToken),
    _isServer(false),
    _isWriter(true),
    _reader(nullptr),
    _writer(nullptr),
    _dataPort(dataPort),
    _commandPort(commandPort + 1),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _commandSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(client)(address=%s, dataPort=%d, commandPort=%d, terminationToken=%d), blocksize is %d", address, dataPort, commandPort, *terminationToken, blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkWriter, reader, blocksize, terminationToken),
    _isServer(false),
    _isWriter(false),
    _reader(reader),
    _writer(nullptr),
    _dataPort(dataPort),
    _commandPort(commandPort),
    _server(address),
    _clientSocket(-1),
    _serverSocket(-1),
    _commandSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(client)(address=%s, dataPort=%d, commandPort=%d, reader=*, terminationToken=%d), blocksize is %d", address, dataPort, commandPort, *terminationToken, blocksize);
    InitClient();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int dataPort, int commandPort, HWriter<T>* writer, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, &_networkReader, blocksize, terminationToken),
    _isServer(true),
    _isWriter(true),
    _reader(nullptr),
    _writer(writer),
    _dataPort(dataPort),
    _commandPort(commandPort + 1),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _commandSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(server)(dataPort=%d, commandPort=%d, writer=*, terminationToken=%d), blocksize is %d", dataPort, commandPort, *terminationToken, blocksize);
    InitServer();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int dataPort, int commandPort, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkReader, blocksize, terminationToken),
    _isServer(true),
    _isWriter(true),
    _reader(nullptr),
    _writer(nullptr),
    _dataPort(dataPort),
    _commandPort(dataPort + 1),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _commandSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(server)(dataPort=%d, commandPort=%d, terminationToken=%d), blocksize is %d", dataPort, commandPort, *terminationToken, blocksize);
    InitServer();
}

template <class T>
HNetworkProcessor<T>::HNetworkProcessor(int dataPort, int commandPort, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(&_networkWriter, reader, blocksize, terminationToken),
    _isServer(true),
    _isWriter(false),
    _reader(reader),
    _writer(nullptr),
    _dataPort(dataPort),
    _commandPort(dataPort + 1),
    _server(NULL),
    _clientSocket(-1),
    _serverSocket(-1),
    _commandSocket(-1),
    _terminated(terminationToken)
{
    HLog("HNetworkProcessor(server)(dataPort=%d, commandPort=%d, reader=*, terminationToken=%d), blocksize is %d", dataPort, commandPort, *terminationToken, blocksize);
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
    if( this->_commandSocket > -1 ) {
        HLog("Closing command socket");
        close(this->_commandSocket);
    }
    HLog("Done");

}

template <class T>
void HNetworkProcessor<T>::InitServer()
{
    // Initialize sockets for incomming connections
    InitServerDataPort();
    InitServerCommandPort();

    // Ignore the SIGPIPE signal since it occurres when ever a client closes the connection
    sigignore(SIGPIPE);
    HLog("SIGPIPE disabled");
}

template <class T>
void HNetworkProcessor<T>::InitServerDataPort()
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
    _address.sin_port = htons( _dataPort );

    // Forcefully attaching socket to the selected port
    if (bind(_serverSocket, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        HError("bind() failed for port %d on INADDR_ANY", _dataPort);
        throw new HInitializationException("bind() failed");
    }
    HLog("Bound to INADDR_ANY on port %d", _dataPort);
}

template <class T>
void HNetworkProcessor<T>::InitServerCommandPort()
{
    if ((this->_commandSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        HError("Failed to create command socket");
        throw new HInitializationException("Failed to create command socket");
    }
    HLog("Created command socket");

    // Forcefully attaching socket to the selected port
    int opt = 0;
    if (setsockopt(_commandSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        HError("setsockopt() failed");
        throw new HInitializationException("setsockopt() failed");
    }

    // Server address
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( _commandPort );

    // Forcefully attaching socket to the selected port
    if (bind(_commandSocket, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        HError("bind() failed for port %d on INADDR_ANY", _commandPort);
        throw new HInitializationException("bind() failed");
    }
    HLog("Bound to INADDR_ANY on port %d", _commandPort);
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
    _address.sin_port = htons(_dataPort);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, _server, &_address.sin_addr)<=0)
    {
        HError("Invalid address %s", _server);
        throw new HNetworkException("Invalid address");
    }

    // Connect
    HLog("Trying to connect to %s:%d", _server, _dataPort);
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

    // Start thread than will listen for commands
    HLog("Creating commandlistener thread");
    std::thread commandListener ( [this] { ReceiveCommands(); } );

    // Run server untill terminated
    try
    {
        int addrlen = sizeof(_address);

        // Run untill stopped
        while(!*_terminated)
        {
            // Wait for connection
            HLog("Listening on port %d", _dataPort);
            if (listen(_serverSocket, 3) < 0)
            {
                HError("Error in listen(), socket may have been closed");
                HLog("Exit from Run() due to error");
                break;
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
                    break;
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
                break;
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
    }

    // Wait for the command listener thread to halt
    HLog("Waiting for commandListener thread to halt");
    commandListener.join();
    HLog("Commandlistenener thread halted");

    // Server stopped
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

template <class T>
void HNetworkProcessor<T>::ReceiveCommands()
{
    // Prepare select()
    int activity;
    fd_set rfds;
    struct timeval tv;
    int clientCommandSocket = -1;

    // Run commandlistener untill terminated
    HLog("commandlistener thread starting");
    try
    {
        int addrlen = sizeof(_address);

        // Run untill stopped
        while(!*_terminated)
        {
            // Wait for connection
            HLog("Listening on port %d", _commandPort);
            if (listen(_commandSocket, 3) < 0)
            {
                HError("Error in listen(), socket may have been closed");
                HLog("Exit from Run() due to error");
                break;
            }

            //wait for activity (connect)
            while( !*_terminated )
            {
                // Wait for activity on the commandsocket.
                // This is not the optimal way to check for exit, but it will suffice
                // for the types of scenarios that we will encounter, running on small
                // isolated boxes (raspberry's etc.) or just handling a very limited number
                // of connections. Some days we'll make this beautifull though.
                tv.tv_sec = 3;
                tv.tv_usec = 0;
                FD_ZERO(&rfds);
                FD_SET(_commandSocket, &rfds);
                activity = select( _commandSocket + 1 , &rfds , &rfds , NULL , &tv);
                if ( activity < 0)
                {
                    HLog("Error when waiting for connections");
                    break;
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
            HLog("Accepting command connection");

            if ((clientCommandSocket = accept(_commandSocket, (struct sockaddr *)&_address, (socklen_t*)&addrlen))<0)
            {
                HError("Error in accept, socket may have been closed");
                HLog("Exit from Run() due to error");
                break;
            }
            HLog("Accepting new command connection from %s", inet_ntoa(((struct sockaddr_in *) &_address)->sin_addr));

            // Read and handle the command
            ReadCommand(clientCommandSocket);

            // Close the socket, should it still be open
            if( clientCommandSocket > -1 )
            {
                HLog("Closing command connection to the client");
                close(clientCommandSocket);
            }
            HLog("Command connection closed");
        }
    }
    catch( const std::exception& ex )
    {
        HError("Caught exception while handling client command: %s", ex.what());
        if( _clientSocket > -1 )
        {
            HLog("Closing connection to the client");
            close(_clientSocket);
            _clientSocket = -1;
        }
    }
    HLog("commandlistener thread stopping");
}

template <class T>
void HNetworkProcessor<T>::ReadCommand(int socket)
{
    HNetworkReader<int8_t> reader(socket);
    HNetworkWriter<int8_t> writer(socket);
    int8_t FalseStatus = 0;
    int8_t TrueStatus = 1;

    // We do not know how much data is needed for the command, so initially read enough bytes for the basic command struct
    int8_t commandData[HCommandMinimumsSize];
    int length = reader.Read(commandData, HCommandMinimumsSize);
    if( length != HCommandMinimumsSize ){
        HError("Could not read enough bytes for a HCommand struct");
        writer.Write(&FalseStatus, 1);
        return;
    }
    HCommand cmd;
    memcpy((void*) &cmd, (void*) commandData, HCommandMinimumsSize);
    HLog("Received command with class %d, opcode %d and length %d", cmd.Class, cmd.Opcode, cmd.Length);

    // If length is larger than 0, then cmd.Data.Content contains a pointer to some data.
    // This is handled by putting the data on the network after the command structure, then
    // reading the data and reconstructing the HCommandData struct now pointing to local data
    if( cmd.Length > 0 )
    {
        HLog("Reading %d bytes of commanddata content from the network", cmd.Length);
        cmd.Data.Content = (void*) new int8_t[cmd.Length];
        length = reader.Read((int8_t *) cmd.Data.Content, cmd.Length);
        if( length != cmd.Length ){
            HError("Could not read enough bytes for the HCommandData.Content field");
            writer.Write(&FalseStatus, 1);
            delete[] (int8_t*) cmd.Data.Content;
            return;
        }
    }

    // Ship the command
    if( HProcessor<T>::_reader != nullptr )
    {
        HLog("Sending command to reader chain");
        if( !HProcessor<T>::_reader->Command(&cmd) )
        {
            HError("Error when sending command to reader chain");
            writer.Write(&FalseStatus, 1);
            if( cmd.Length > 0 )
            {
                delete[] (int8_t*) cmd.Data.Content;
            }
            return;
        }
    }
    if( HProcessor<T>::_writer != nullptr )
    {
        HLog("Sending command to writer chain");
        if( !HProcessor<T>::_writer->Command(&cmd) )
        {
            HError("Error when sending command to writer chain");
            writer.Write(&FalseStatus, 1);
            if( cmd.Length > 0 )
            {
                delete[] (int8_t*)  cmd.Data.Content;
            }
            return;
        }
    }
    HLog("Command sent to local reader and/or writer (if any)");
    writer.Write(&TrueStatus, 1);
    if( cmd.Length > 0 )
    {
        delete[] (int8_t*) cmd.Data.Content;
    }
}

template <class T>
bool HNetworkProcessor<T>::SendCommand(HCommand* command)
{
    // Create a socket
    int commandSocket;
    if ((commandSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        HError("Failed to create command socket");
        throw new HInitializationException("Failed to create command socket");
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
    _address.sin_port = htons(_commandPort);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, _server, &_address.sin_addr)<=0)
    {
        HError("Invalid address %s", _server);
        throw new HNetworkException("Invalid address");
    }

    // Connect
    HLog("Trying to connect to %s:%d", _server, _commandPort);
    if (connect(commandSocket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
        HError("Failed to connect to command port on server %s", _server);
        throw new HNetworkException("Failed to connect to command port on server");
    }
    HLog("Connected");

    // Send the command
    HNetworkWriter<int8_t> writer(commandSocket);
    int8_t commandData[HCommandMinimumsSize + command->Length];
    memcpy((void*) commandData, (void*) command, HCommandMinimumsSize);
    memcpy((void*) &commandData[HCommandMinimumsSize], (void*) command->Data.Content, command->Length);
    int length = writer.Write(commandData, HCommandMinimumsSize + command->Length);
    if( length != HCommandMinimumsSize + command->Length )
    {
        HError("Incorrect length of data written to the network. Expected %d send %d", HCommandMinimumsSize + command->Length, length);
        close(commandSocket);
        return false;
    }

    // Read the return byte indicating error or success
    HNetworkReader<int8_t> reader(commandSocket);
    int8_t commandStatus;
    length = reader.Read(&commandStatus, 1);
    if( length != 1 )
    {
        HError("Read wrong length of command status. Expected 1 got %d", length);
        close(commandSocket);
        return false;
    }

    // Disconnect
    close(commandSocket);
    HLog("Commandstatus is %d", commandStatus);
    return commandStatus == 1;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HNetworkProcessor()
template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HWriter<int8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HWriter<uint8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HWriter<int16_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HWriter<int32_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int dataPort, int commandPort, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(int dataPort, int commandPort, HWriter<int8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int dataPort, int commandPort, HWriter<uint8_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(int dataPort, int commandPort, HWriter<int16_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(int dataPort, int commandPort, HWriter<int32_t>* writer, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(int dataPort, int commandPort, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int8_t>::HNetworkProcessor(int dataPort, int commandPort, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int dataPort, int commandPort, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int16_t>::HNetworkProcessor(int dataPort, int commandPort, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HNetworkProcessor<int32_t>::HNetworkProcessor(int dataPort, int commandPort, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

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

// SendCommand()
template
bool HNetworkProcessor<int8_t>::SendCommand(HCommand* command);

template
bool HNetworkProcessor<uint8_t>::SendCommand(HCommand* command);

template
bool HNetworkProcessor<int16_t>::SendCommand(HCommand* command);

template
bool HNetworkProcessor<int32_t>::SendCommand(HCommand* command);

// ReceiveCommands()
template
void HNetworkProcessor<int8_t>::ReceiveCommands();

template
void HNetworkProcessor<uint8_t>::ReceiveCommands();

template
void HNetworkProcessor<int16_t>::ReceiveCommands();

template
void HNetworkProcessor<int32_t>::ReceiveCommands();

//! @endcond
#endif