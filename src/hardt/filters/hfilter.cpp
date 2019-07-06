#ifndef __HFILTER_CPP
#define __HFILTER_CPP

#include "hwriter.h"
#include "hreader.h"
#include "hfilter.h"

template <class T>
HFilter<T>::HFilter(HWriter<T>* writer, size_t blocksize):
    _writer(writer),
    _reader(NULL),
    _blocksize(blocksize)
{
    HLog("HFilter(HWriter*)");

    _buffer = new T[blocksize];
    HLog("Allocated %d as local buffer", blocksize * sizeof(T));
}

template <class T>
HFilter<T>::HFilter(HReader<T>* reader, size_t blocksize):
    _writer(NULL),
    _reader(reader),
    _blocksize(blocksize)
{
    HLog("HFilter(HReader*)");

    _buffer = new T[blocksize];
    HLog("Allocated %d as local buffer", blocksize * sizeof(T));
}

template <class T>
HFilter<T>::~HFilter()
{
    HLog("~HFilter()");
    delete _buffer;
}

template <class T>
int HFilter<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HFilter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not allowed to write more data than the size given at creation of the filter");
    }

    Filter(src, _buffer, blocksize);
    return _writer->Write(_buffer, blocksize);
}

template <class T>
int HFilter<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HFilter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation of the filter");
    }

    int received = _reader->Read(_buffer, blocksize);
    Filter(_buffer, dest, received);
    return received;
}

template <class T>
bool HFilter<T>::Start()
{
    if( _reader != NULL )
    {
        HLog("Calling Start() on reader");
        return _reader->Start();
    }
    if( _writer != NULL )
    {
        HLog("Calling Start() on writer");
        return _writer->Start();
    }
    return false;
}

template <class T>
bool HFilter<T>::Stop()
{
    if( _reader != NULL )
    {
        HLog("Calling Stop() on reader");
        return _reader->Stop();
    }
    if( _writer != NULL )
    {
        HLog("Calling Stop() on writer");
        return _writer->Stop();
    }
    return false;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HFilter
template
HFilter<int8_t>::HFilter(HWriter<int8_t>* writer, size_t blocksize);

template
HFilter<uint8_t>::HFilter(HWriter<uint8_t>* writer, size_t blocksize);

template
HFilter<int16_t>::HFilter(HWriter<int16_t>* writer, size_t blocksize);

template
HFilter<int32_t>::HFilter(HWriter<int32_t>* writer, size_t blocksize);

template
HFilter<int8_t>::HFilter(HReader<int8_t>* reader, size_t blocksize);

template
HFilter<uint8_t>::HFilter(HReader<uint8_t>* reader, size_t blocksize);

template
HFilter<int16_t>::HFilter(HReader<int16_t>* reader, size_t blocksize);

template
HFilter<int32_t>::HFilter(HReader<int32_t>* reader, size_t blocksize);

// ~HFilter()
template
HFilter<int8_t>::~HFilter();

template
HFilter<uint8_t>::~HFilter();

template
HFilter<int16_t>::~HFilter();

template
HFilter<int32_t>::~HFilter();

// Write()
template
int HFilter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HFilter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HFilter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HFilter<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HFilter<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HFilter<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HFilter<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HFilter<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif