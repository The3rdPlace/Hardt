#ifndef __HMUX_CPP
#define __HMUX_CPP

#include "hmux.h"

template <class T>
HMux<T>::HMux( std::vector< HReader<T>* > readers, size_t blocksize):
    _blocksize(blocksize),
    _readers(readers),
    _writer(nullptr),
    _bufferCount(readers.size()),
    _output(nullptr)
{
    HLog("HMux(%d readers)", readers.size());

    _buffers = new T*[readers.size()];
    for( int i = 0; i < readers.size(); i++ )
    {
        _buffers[i] = new T[blocksize / readers.size()];
    }
}

template <class T>
HMux<T>::HMux( HWriter<T>* writer, int writers, size_t blocksize):
    _blocksize(blocksize),
    _writers(writers),
    _writer(writer),
    _buffers(nullptr),
    _bufferCount(writers),
    _received(0) {

    HLog("HMux(writer)");

    _output = new T[blocksize * _writers];
    _buffers = new T*[writers];
    for( int i = 0; i < writers; i++ )
    {
        _buffers[i] = new T[blocksize];
    }
}


template <class T>
HMux<T>::HMux( std::vector< HWriterConsumer<T>* > consumers, size_t blocksize):
        _blocksize(blocksize),
        _writers(consumers.size()),
        _writer(nullptr),
        _buffers(nullptr),
        _bufferCount(consumers.size()),
        _received(0) {

    HLog("HMux(%d consumers)", consumers.size());

    _output = new T[blocksize * _writers];
    _buffers = new T*[consumers.size()];
    for( int i = 0; i < consumers.size(); i++ )
    {
        _buffers[i] = new T[blocksize];
        consumers[i]->SetWriter(this->Writer());
    }
}

template <class T>
HMux<T>::~HMux()
{
    HLog("~HMux()");
    if( _buffers != nullptr ) {
        for (int i = 0; i < _bufferCount; i++) {
            delete _buffers[i];
        }
        delete _buffers;
    }
    if( _output != nullptr ) {
        delete _output;
    }
}

template <class T>
int HMux<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize != _blocksize ) {
        HError("Incorrect blocksize in read. Expected %d but %d was requested", _blocksize, blocksize);
        throw new HReaderIOException("Incorrect blocksize in Read()");
    }

    for( int i = 0; i < _readers.size(); i++ )
    {
        int read = _readers[i]->Read(_buffers[i], blocksize / _readers.size() );
        if( read != (blocksize / _readers.size() ) )
        {
            HError("Incorrect read with size %d for reader %d, returning zero", read, i);
            return 0;
        }
    }

    int pos = 0;
    int reader = 0;
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = _buffers[reader++][pos];
        if( reader >= _readers.size() )
        {
            reader = 0;
            pos++;
        }
    }

    return blocksize;
}

template <class T>
int HMux<T>::Write(T* src, size_t blocksize) {

    if( blocksize != _blocksize ) {
        HError("Incorrect blocksize in write. Expected %d but %d was requested", _blocksize, blocksize);
        throw new HReaderIOException("Incorrect blocksize in Write()");
    }

    memcpy((void*) _buffers[_received++], (void*) src, blocksize * sizeof(T));

    if( _received == _writers ) {
        int pos = 0;
        int writer = 0;
        for( int i = 0; i < _blocksize * _writers; i++ )
        {
            _output[i] = _buffers[writer++][pos];
            if( writer >= _writers )
            {
                writer = 0;
                pos++;
            }
        }
        _writer->Write(_output, _blocksize * _writers);
        _received = 0;
    }

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HMux
template
HMux<int8_t>::HMux( std::vector< HReader<int8_t>* > readers, size_t blocksize);

template
HMux<uint8_t>::HMux( std::vector< HReader<uint8_t>* > readers, size_t blocksize);

template
HMux<int16_t>::HMux( std::vector< HReader<int16_t>* > readers, size_t blocksize);

template
HMux<int32_t>::HMux( std::vector< HReader<int32_t>* > readers, size_t blocksize);

template
HMux<int8_t>::HMux( HWriter<int8_t>* writer, int writers, size_t blocksize);

template
HMux<uint8_t>::HMux( HWriter<uint8_t>* writer, int writers, size_t blocksize);

template
HMux<int16_t>::HMux( HWriter<int16_t>* writer, int writers, size_t blocksize);

template
HMux<int32_t>::HMux( HWriter<int32_t>* writer, int writers, size_t blocksize);

template
HMux<int8_t>::HMux( std::vector< HWriterConsumer<int8_t>* > consumers, size_t blocksize);

template
HMux<uint8_t>::HMux( std::vector< HWriterConsumer<uint8_t>* > consumers, size_t blocksize);

template
HMux<int16_t>::HMux( std::vector< HWriterConsumer<int16_t>* > consumers, size_t blocksize);

template
HMux<int32_t>::HMux( std::vector< HWriterConsumer<int32_t>* > consumers, size_t blocksize);

// ~HMux()
template
HMux<int8_t>::~HMux();

template
HMux<uint8_t>::~HMux();

template
HMux<int16_t>::~HMux();

template
HMux<int32_t>::~HMux();

// Read
template
int HMux<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HMux<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HMux<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HMux<int32_t>::Read(int32_t* dest, size_t blocksize);

// Write
template
int HMux<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HMux<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HMux<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HMux<int32_t>::Write(int32_t* src, size_t blocksize);

//! @endcond
#endif