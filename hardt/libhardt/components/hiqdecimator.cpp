#ifndef __HIQDECIMATOR_CPP
#define __HIQDECIMATOR_CPP

#include <cstring>

#include "hardt.h"
#include "hiqdecimator.h"

template <class T>
HIqDecimator<T>::HIqDecimator(std::string id, HWriter<T>* writer, int factor, size_t blocksize, bool collect):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr),
    _collect(collect),
    _pos(0) {
    HLog("HIqDecimator(HWriter*, blocksize=%d)", blocksize);
    Init();
}

template <class T>
HIqDecimator<T>::HIqDecimator(std::string id, HWriterConsumer<T>* consumer, int factor, size_t blocksize, bool collect):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr),
    _collect(collect),
    _pos(0) {
    HLog("HIqDecimator(HWriterConsumer*, blocksize=%d)", blocksize);
    Init();
    consumer->SetWriter(this);
}

template <class T>
HIqDecimator<T>::HIqDecimator(std::string id, HReader<T>* reader, int factor, size_t blocksize, bool collect):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader),
    _collect(collect),
    _pos(0) {
    HLog("HIqDecimator(HReader*, blocksize=%d)", blocksize);
    Init();
}

template <class T>
HIqDecimator<T>::~HIqDecimator()
{
    delete[] _buffer;
}

template <class T>
void HIqDecimator<T>::Init() {

    if( _factor <= 0 ) {
        throw new HInitializationException("Decimation factor can not be zero or negative");
    }

    _buffer = new T[_blocksize];
}

template <class T>
int HIqDecimator<T>::WriteImpl(T* src, size_t blocksize)
{
    // Decimate
    for(; _pos < blocksize; _pos += _factor * 2) {
        _buffer[_length++] = src[_pos];
        _buffer[_length++] = src[_pos + 1];
    }
    _pos -= blocksize;

    // Write ?
    if( _length == _blocksize || !_collect ) {
        _writer->Write(_buffer, _length);
        _length = 0;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HIqDecimator<T>::ReadImpl(T* dest, size_t blocksize)
{
    // Read
    _length = 0;
    while( _length < _blocksize ) {

        // Read a block
        int read = _reader->Read(_buffer, _blocksize);
        if( read == 0 ) {
            HLog("Received zero-read, returning zero");
            return 0;
        }
        else if( read < _blocksize ) {
            HLog("Received too low read size, inserting silence");
            memset((void*) &_buffer[read], 0, _blocksize - read);

        }
        else if( read > _blocksize ) {
            HError("Received too big read size, memory corruption must be expected. Stopping");
            return 0;
        }

        // Decimate the block
        for(; _pos < _blocksize; _pos += _factor * 2) {
            dest[_length++] = _buffer[_pos];
            dest[_length++] = _buffer[_pos + 1];
        }
        _pos -= blocksize;

        // If not doing a collected read, return what we have now
        if( !_collect ) {
            break;
        }
    }

    // Return complete decimated read
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HIqDecimator()
template
HIqDecimator<int8_t>::HIqDecimator(std::string id, HWriter<int8_t>* writer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<uint8_t>::HIqDecimator(std::string id, HWriter<uint8_t>* writer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int16_t>::HIqDecimator(std::string id, HWriter<int16_t>* writer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int32_t>::HIqDecimator(std::string id, HWriter<int32_t>* writer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int8_t>::HIqDecimator(std::string id, HWriterConsumer<int8_t>* consumer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<uint8_t>::HIqDecimator(std::string id, HWriterConsumer<uint8_t>* consumer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int16_t>::HIqDecimator(std::string id, HWriterConsumer<int16_t>* consumer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int32_t>::HIqDecimator(std::string id, HWriterConsumer<int32_t>* consumer, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int8_t>::HIqDecimator(std::string id, HReader<int8_t>* reader, int factor, size_t blocksize, bool collect);

template
HIqDecimator<uint8_t>::HIqDecimator(std::string id, HReader<uint8_t>* reader, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int16_t>::HIqDecimator(std::string id, HReader<int16_t>* reader, int factor, size_t blocksize, bool collect);

template
HIqDecimator<int32_t>::HIqDecimator(std::string id, HReader<int32_t>* reader, int factor, size_t blocksize, bool collect);

// ~HIqDecimator()
template
HIqDecimator<int8_t>::~HIqDecimator();

template
HIqDecimator<uint8_t>::~HIqDecimator();

template
HIqDecimator<int16_t>::~HIqDecimator();

template
HIqDecimator<int32_t>::~HIqDecimator();

// Init()
template
void HIqDecimator<int8_t>::Init();

template
void HIqDecimator<uint8_t>::Init();

template
void HIqDecimator<int16_t>::Init();

template
void HIqDecimator<int32_t>::Init();

// Write()
template
int HIqDecimator<int8_t>::WriteImpl(int8_t* src, size_t blocksize);

template
int HIqDecimator<uint8_t>::WriteImpl(uint8_t* src, size_t blocksize);

template
int HIqDecimator<int16_t>::WriteImpl(int16_t* src, size_t blocksize);

template
int HIqDecimator<int32_t>::WriteImpl(int32_t* src, size_t blocksize);

// Read()
template
int HIqDecimator<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HIqDecimator<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HIqDecimator<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HIqDecimator<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif
