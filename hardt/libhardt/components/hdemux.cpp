#ifndef __HDEMUX_CPP
#define __HDEMUX_CPP

#include <vector>

#include "hardt.h"
#include "hwriter.h"
#include "hreader.h"
#include "hwriterconsumer.h"

#include "hdemux.h"

template <class T>
HDeMux<T>::HDeMux(std::string id, std::vector< HWriter<T>* > writers, size_t blocksize):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _writers(writers),
    _reader(nullptr),
    _blocksize(blocksize),
    _readers(0),
    _nextReader(0),
    _readBuffer(nullptr)
{
    HLog("HDeMux(%d writers)", writers.size());

    _buffers = new T*[writers.size()];
    for( int i = 0; i < writers.size(); i++ )
    {
        _buffers[i] = new T[blocksize / writers.size()];
    }
}

template <class T>
HDeMux<T>::HDeMux(std::string id, HWriterConsumer<T>* consumer, size_t blocksize):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _buffers(NULL),
    _reader(nullptr),
    _blocksize(blocksize),
    _readers(0),
    _nextReader(0),
    _readBuffer(nullptr)
{
    HLog("HDeMux(consumer)");

    consumer->SetWriter(this);
}

template <class T>
HDeMux<T>::HDeMux(std::string id, HReader<T>* reader, int readers, size_t blocksize):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _buffers(NULL),
    _reader(reader),
    _blocksize(blocksize),
    _readers(readers),
    _nextReader(0)
{
    HLog("HDeMux(reader)");
    _readBuffer = new T[blocksize * readers];
}

template <class T>
HDeMux<T>::~HDeMux()
{
    HLog("~HDemux()");
    for(int i = 0; i < _writers.size(); i++ )
    {
        delete _buffers[i];
    }
    delete _buffers;
    if( _readBuffer != nullptr ) {
        delete _readBuffer;
    }
}

template <class T>
int HDeMux<T>::Write(T* src, size_t blocksize)
{
    int pos = 0;
    int writer = 0;
    for( int i = 0; i < blocksize; i++ )
    {
        _buffers[writer++][pos] = src[i];
        if( writer >= _writers.size() )
        {
            writer = 0;
            pos++;
        }
    }

    for( int i = 0; i < _writers.size(); i++ )
    {
        int written = _writers[i]->Write(_buffers[i], blocksize / _writers.size() );
        if( written != (blocksize / _writers.size() ) )
        {
            HError("Incorrect write with size %d for writer %d, returning zero", written, writer);
            return 0;
        }
    }

    return blocksize;
}

template <class T>
int HDeMux<T>::Read(T* dest, size_t blocksize)
{
    if( _nextReader == 0 ) {
        if( _reader->Read(_readBuffer, blocksize * _readers) == 0 ) {
            HLog("Zero length read. Returning 0");
            return 0;
        }
    }

    int j = 0;
    for( int i = 0; i < blocksize * _readers; i += _readers ) {
        dest[j++] = _readBuffer[_nextReader + i];
    }

    _nextReader++;
    if( _nextReader >= _readers ) {
        _nextReader = 0;
    }

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HDeMux
template
HDeMux<int8_t>::HDeMux(std::string id, std::vector< HWriter<int8_t>* > writers, size_t blocksize);

template
HDeMux<uint8_t>::HDeMux(std::string id, std::vector< HWriter<uint8_t>* > writers, size_t blocksize);

template
HDeMux<int16_t>::HDeMux(std::string id, std::vector< HWriter<int16_t>* > writers, size_t blocksize);

template
HDeMux<int32_t>::HDeMux(std::string id, std::vector< HWriter<int32_t>* > writers, size_t blocksize);

template
HDeMux<int8_t>::HDeMux(std::string id, HWriterConsumer<int8_t>* consumer, size_t blocksize);

template
HDeMux<uint8_t>::HDeMux(std::string id, HWriterConsumer<uint8_t>* consumer, size_t blocksize);

template
HDeMux<int16_t>::HDeMux(std::string id, HWriterConsumer<int16_t>* consumer, size_t blocksize);

template
HDeMux<int32_t>::HDeMux(std::string id, HWriterConsumer<int32_t>* consumer, size_t blocksize);

template
HDeMux<int8_t>::HDeMux(std::string id, HReader<int8_t>* reader, int readers, size_t blocksize);

template
HDeMux<uint8_t>::HDeMux(std::string id, HReader<uint8_t>* reader, int readers, size_t blocksize);

template
HDeMux<int16_t>::HDeMux(std::string id, HReader<int16_t>* reader, int readers, size_t blocksize);

template
HDeMux<int32_t>::HDeMux(std::string id, HReader<int32_t>* reader, int readers, size_t blocksize);

// ~HDeMux()
template
HDeMux<int8_t>::~HDeMux();

template
HDeMux<uint8_t>::~HDeMux();

template
HDeMux<int16_t>::~HDeMux();

template
HDeMux<int32_t>::~HDeMux();

// Write
template
int HDeMux<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HDeMux<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HDeMux<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HDeMux<int32_t>::Write(int32_t* src, size_t blocksize);

// Read
template
int HDeMux<int8_t>::Read(int8_t * dest, size_t blocksize);

template
int HDeMux<uint8_t>::Read(uint8_t * dest, size_t blocksize);

template
int HDeMux<int16_t>::Read(int16_t * dest, size_t blocksize);

template
int HDeMux<int32_t>::Read(int32_t * dest, size_t blocksize);

//! @endcond
#endif