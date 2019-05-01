#ifndef __HFILEWRITER_CPP
#define __HFILEWRITER_CPP

#include "hfilewriter.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HFileWriter<T>::HFileWriter(const char* filename):
    _filename(filename)
{}

template <class T>
int HFileWriter<T>::Write(T* src, size_t blocksize)
{
    this->Metrics.Writes++;
    _stream.write((char*) src, blocksize * sizeof(T));
    this->Metrics.BlocksOut += blocksize;
    this->Metrics.BytesOut += blocksize * sizeof(T);
    return blocksize;
}

template <class T>
void HFileWriter<T>::Seek(int bytes)
{
    _stream.seekp(bytes, std::ios::beg);
}

template <class T>
bool HFileWriter<T>::Start(void* data)
{
    HLog("Trying to open stream for %s", _filename);
    _stream.open(_filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if( !_stream.is_open())
    {
        HError("Failed to open file %s", _filename);
        return false;
    }
    HLog("Stream is open");
    return true;
}

template <class T>
bool HFileWriter<T>::Stop()
{
    HLog("Closing stream");
    _stream.close();
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HFileWriter()
template
HFileWriter<char>::HFileWriter(const char* path);

template
HFileWriter<unsigned char>::HFileWriter(const char* path);

template
HFileWriter<int>::HFileWriter(const char* path);

template
HFileWriter<int8_t>::HFileWriter(const char* path);

template
HFileWriter<int16_t>::HFileWriter(const char* path);

// Start()
template
bool HFileWriter<char>::Start(void* data);

template
bool HFileWriter<unsigned char>::Start(void* data);

template
bool HFileWriter<int>::Start(void* data);

template
bool HFileWriter<int8_t>::Start(void* data);

template
bool HFileWriter<int16_t>::Start(void* data);

// Stop()
template
bool HFileWriter<char>::Stop();

template
bool HFileWriter<unsigned char>::Stop();

template
bool HFileWriter<int>::Stop();

template
bool HFileWriter<int8_t>::Stop();

template
bool HFileWriter<int16_t>::Stop();

// Write()
template
int HFileWriter<char>::Write(char* dest, size_t blocksize);

template
int HFileWriter<unsigned char>::Write(unsigned char* dest, size_t blocksize);

template
int HFileWriter<int>::Write(int* dest, size_t blocksize);

template
int HFileWriter<int8_t>::Write(int8_t* dest, size_t blocksize);

template
int HFileWriter<int16_t>::Write(int16_t* dest, size_t blocksize);

#endif
