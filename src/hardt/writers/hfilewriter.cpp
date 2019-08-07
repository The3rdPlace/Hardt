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
HFileWriter<T>::HFileWriter(const char* filename, HWriterConsumer<T>* consumer):
    _filename(filename)
{
    consumer->SetWriter(this);
}

template <class T>
int HFileWriter<T>::Write(T* src, size_t blocksize)
{
    this->Metrics.Writes++;
    _stream.write((char*) src, blocksize * sizeof(T));
    this->Metrics.BlocksOut++;
    this->Metrics.BytesOut += blocksize * sizeof(T);
    return blocksize;
}

template <class T>
void HFileWriter<T>::Seek(int bytes)
{
    _stream.seekp(bytes, std::ios::beg);
}

template <class T>
bool HFileWriter<T>::Start()
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
HFileWriter<int8_t>::HFileWriter(const char* path);

template
HFileWriter<uint8_t>::HFileWriter(const char* path);

template
HFileWriter<int16_t>::HFileWriter(const char* path);

template
HFileWriter<int32_t>::HFileWriter(const char* path);

template
HFileWriter<long>::HFileWriter(const char* path);

template
HFileWriter<double>::HFileWriter(const char* path);

template
HFileWriter<int8_t>::HFileWriter(const char* path, HWriterConsumer<int8_t>* consumer);

template
HFileWriter<uint8_t>::HFileWriter(const char* path, HWriterConsumer<uint8_t>* consumer);

template
HFileWriter<int16_t>::HFileWriter(const char* path, HWriterConsumer<int16_t>* consumer);

template
HFileWriter<int32_t>::HFileWriter(const char* path, HWriterConsumer<int32_t>* consumer);

template
HFileWriter<long>::HFileWriter(const char* path, HWriterConsumer<long>* consumer);

template
HFileWriter<double>::HFileWriter(const char* path, HWriterConsumer<double>* consumer);

// Start()
template
bool HFileWriter<int8_t>::Start();

template
bool HFileWriter<uint8_t>::Start();

template
bool HFileWriter<int16_t>::Start();

template
bool HFileWriter<int32_t>::Start();

template
bool HFileWriter<long>::Start();

template
bool HFileWriter<double>::Start();

// Stop()
template
bool HFileWriter<int8_t>::Stop();

template
bool HFileWriter<uint8_t>::Stop();

template
bool HFileWriter<int16_t>::Stop();

template
bool HFileWriter<int32_t>::Stop();

template
bool HFileWriter<long>::Stop();

template
bool HFileWriter<double>::Stop();

// Write()
template
int HFileWriter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HFileWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HFileWriter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HFileWriter<int32_t>::Write(int32_t* src, size_t blocksize);

template
int HFileWriter<long>::Write(long* src, size_t blocksize);

template
int HFileWriter<double>::Write(double* src, size_t blocksize);

#endif
