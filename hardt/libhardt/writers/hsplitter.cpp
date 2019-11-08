#ifndef __HSPLITTER_CPP
#define __HSPLITTER_CPP

#include "hsplitter.h"

template <class T>
int HSplitter<T>::Write(T* src, size_t blocksize)
{
    for( typename std::vector< HWriter<T>* >::iterator it = _writers.begin(); it != _writers.end(); it++ )
    {
        int written = (*it)->Write(src, blocksize);
        if( written != blocksize )
        {
            HError("Incorrect write to writer (%d blocks)", written);
            return 0;
        }
    }

    return blocksize;
}

template <class T>
bool HSplitter<T>::Start()
{
    HLog("Starting splitter writers");
    for( typename std::vector< HWriter<T>* >::iterator it = _writers.begin(); it != _writers.end(); it++ )
    {
        HLog("Starting one writer");
        if( !(*it)->Start() ) {
            HLog("Writer failed to start");
            return false;
        }
        HLog("Writer started");
    }
    HLog("All writers started");
    return true;
}

template <class T>
bool HSplitter<T>::Stop()
{
    HLog("Stopping splitter writers");
    for( typename std::vector< HWriter<T>* >::iterator it = _writers.begin(); it != _writers.end(); it++ )
    {
        HLog("Stopping one writer");
        if( !(*it)->Stop() ) {
            HLog("Writer failed to stop");
            return false;
        }
        HLog("Writer stopped");
    }
    HLog("All writers stopped");
    return true;
}

template <class T>
void HSplitter<T>::SetWriter(HWriter<T>* writer)
{
    HLog("HSplitter::SetWriter(HWriter*)");
    for( typename std::vector< HWriter<T>* >::iterator it = _writers.begin(); it != _writers.end(); it++ )
    {
        if( writer == (*it) ) {
            HLog("Writer is already registered, ignoring");
            return;
        }
    }
    _writers.push_back(writer);
    HLog("Added writer");
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Write()
template
int HSplitter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HSplitter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HSplitter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HSplitter<int32_t>::Write(int32_t* src, size_t blocksize);

// Start()
template
bool HSplitter<int8_t>::Start();

template
bool HSplitter<uint8_t>::Start();

template
bool HSplitter<int16_t>::Start();

template
bool HSplitter<int32_t>::Start();

// Stop()
template
bool HSplitter<int8_t>::Stop();

template
bool HSplitter<uint8_t>::Stop();

template
bool HSplitter<int16_t>::Stop();

template
bool HSplitter<int32_t>::Stop();

// SetWriter()
template
void HSplitter<int8_t>::SetWriter(HWriter<int8_t>* writer);

template
void HSplitter<uint8_t>::SetWriter(HWriter<uint8_t>* writer);

template
void HSplitter<int16_t>::SetWriter(HWriter<int16_t>* writer);

template
void HSplitter<int32_t>::SetWriter(HWriter<int32_t>* writer);

//! @endcond
#endif