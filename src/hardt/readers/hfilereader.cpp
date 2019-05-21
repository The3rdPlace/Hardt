#ifndef __HFILEREADER_CPP
#define __HFILEREADER_CPP

#include "hfilereader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HFileReader<T>::HFileReader(const char* filename):
    _filename(filename)
{}

template <class T>
int HFileReader<T>::Read(T* dest, size_t blocksize)
{
    // Check for eof
    if( _stream.eof() )
    {
        HLog("At eof. Returning zero read");
        return 0;
    }

    // Read next chunk
    this->Metrics.Reads++;
    _stream.read((char*) dest, blocksize * sizeof(T));
    this->Metrics.BlocksIn += blocksize;
    this->Metrics.BytesIn += blocksize * sizeof(T);
    return blocksize;
}

template <class T>
void HFileReader<T>::Seek(int bytes)
{
    _stream.seekg(bytes, std::ios::beg);
}

template <class T>
bool HFileReader<T>::Start(void* data)
{
    HLog("Trying to open stream for %s", _filename);
    _stream.open(_filename, std::ios::binary);
    if( !_stream.is_open())
    {
        HError("Failed to open file %s", _filename);
        return false;
    }
    HLog("Stream is open");
    return true;
}

template <class T>
bool HFileReader<T>::Stop()
{
    HLog("Closing stream");
    _stream.close();
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HFileReader()
template
HFileReader<int8_t>::HFileReader(const char* path);

template
HFileReader<uint8_t>::HFileReader(const char* path);

template
HFileReader<int16_t>::HFileReader(const char* path);

template
HFileReader<int32_t>::HFileReader(const char* path);

// Start()
template
bool HFileReader<int8_t>::Start(void* data);

template
bool HFileReader<uint8_t>::Start(void* data);

template
bool HFileReader<int16_t>::Start(void* data);

template
bool HFileReader<int32_t>::Start(void* data);

// Stop()
template
bool HFileReader<int8_t>::Stop();

template
bool HFileReader<uint8_t>::Stop();

template
bool HFileReader<int16_t>::Stop();

template
bool HFileReader<int32_t>::Stop();

// Read()
template
int HFileReader<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HFileReader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HFileReader<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HFileReader<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif
