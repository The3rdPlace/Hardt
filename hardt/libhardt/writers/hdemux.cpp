#ifndef __HDEMUX_CPP
#define __HDEMUX_CPP

#include "hdemux.h"

template <class T>
HDeMux<T>::HDeMux( std::vector< HWriter<T>* > writers, size_t blocksize):
    _writers(writers),
    _blocksize(blocksize)
{
    HLog("HDeMux(%d writers)", writers.size());

    _buffers = new T*[writers.size()];
    for( int i = 0; i < writers.size(); i++ )
    {
        _buffers[i] = new T[blocksize / writers.size()];
    }
}

template <class T>
HDeMux<T>::HDeMux( HWriterConsumer<T>* consumer, size_t blocksize):
    _buffers(NULL),
    _blocksize(blocksize)
{
    HLog("HDeMux(consumer)");

    consumer->SetWriter(this);
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

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HDeMux
template
HDeMux<int8_t>::HDeMux( std::vector< HWriter<int8_t>* > writers, size_t blocksize);

template
HDeMux<uint8_t>::HDeMux( std::vector< HWriter<uint8_t>* > writers, size_t blocksize);

template
HDeMux<int16_t>::HDeMux( std::vector< HWriter<int16_t>* > writers, size_t blocksize);

template
HDeMux<int32_t>::HDeMux( std::vector< HWriter<int32_t>* > writers, size_t blocksize);

template
HDeMux<int8_t>::HDeMux( HWriterConsumer<int8_t>* consumer, size_t blocksize);

template
HDeMux<uint8_t>::HDeMux( HWriterConsumer<uint8_t>* consumer, size_t blocksize);

template
HDeMux<int16_t>::HDeMux( HWriterConsumer<int16_t>* consumer, size_t blocksize);

template
HDeMux<int32_t>::HDeMux( HWriterConsumer<int32_t>* consumer, size_t blocksize);

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

//! @endcond
#endif