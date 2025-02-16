#ifndef __HDECIMATOR_CPP
#define __HDECIMATOR_CPP

#include <cstring>

#include "hardt.h"
#include "hdecimator.h"

template <class T>
HDecimator<T>::HDecimator(std::string id, HWriter<T>* writer, int factor, size_t blocksize, bool collect, int start):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr),
    _collect(collect),
    _pos(start) {
    HLog("HDecimator(HWriter*, blocksize=%d)", blocksize);
    Init();
}

template <class T>
HDecimator<T>::HDecimator(std::string id, HWriterConsumer<T>* consumer, int factor, size_t blocksize, bool collect, int start):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr),
    _collect(collect),
    _pos(start) {

    HLog("HDecimator(HWriterConsumer*, blocksize=%d)", blocksize);
    Init();
    consumer->SetWriter(this);
}

template <class T>
HDecimator<T>::HDecimator(std::string id, HReader<T>* reader, int factor, size_t blocksize, bool collect, int start):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader),
    _collect(collect),
    _pos(start) {
    HLog("HDecimator(HReader*, blocksize=%d)", blocksize);
    Init();
}

template <class T>
HDecimator<T>::~HDecimator()
{
    delete[] _buffer;
}

template <class T>
void HDecimator<T>::Init() {

    if( _factor <= 0 ) {
        throw new HInitializationException("Decimation factor can not be zero or negative");
    }

    _buffer = new T[_blocksize];
}

template <class T>
int HDecimator<T>::WriteImpl(T* src, size_t blocksize)
{
    // Decimate
    for(; _pos < blocksize; _pos += _factor) {
        _buffer[_length++] = src[_pos];
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
int HDecimator<T>::ReadImpl(T* dest, size_t blocksize)
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
        for(; _pos < _blocksize; _pos += _factor) {
            dest[_length++] = _buffer[_pos];
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

// HDecimator()
template
HDecimator<int8_t>::HDecimator(std::string id, HWriter<int8_t>* writer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<uint8_t>::HDecimator(std::string id, HWriter<uint8_t>* writer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int16_t>::HDecimator(std::string id, HWriter<int16_t>* writer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int32_t>::HDecimator(std::string id, HWriter<int32_t>* writer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int8_t>::HDecimator(std::string id, HWriterConsumer<int8_t>* consumer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<uint8_t>::HDecimator(std::string id, HWriterConsumer<uint8_t>* consumer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int16_t>::HDecimator(std::string id, HWriterConsumer<int16_t>* consumer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int32_t>::HDecimator(std::string id, HWriterConsumer<int32_t>* consumer, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int8_t>::HDecimator(std::string id, HReader<int8_t>* reader, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<uint8_t>::HDecimator(std::string id, HReader<uint8_t>* reader, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int16_t>::HDecimator(std::string id, HReader<int16_t>* reader, int factor, size_t blocksize, bool collect, int start);

template
HDecimator<int32_t>::HDecimator(std::string id, HReader<int32_t>* reader, int factor, size_t blocksize, bool collect, int start);

// ~HDecimator()
template
HDecimator<int8_t>::~HDecimator();

template
HDecimator<uint8_t>::~HDecimator();

template
HDecimator<int16_t>::~HDecimator();

template
HDecimator<int32_t>::~HDecimator();

// Init()
template
void HDecimator<int8_t>::Init();

template
void HDecimator<uint8_t>::Init();

template
void HDecimator<int16_t>::Init();

template
void HDecimator<int32_t>::Init();

// Write()
template
int HDecimator<int8_t>::WriteImpl(int8_t* src, size_t blocksize);

template
int HDecimator<uint8_t>::WriteImpl(uint8_t* src, size_t blocksize);

template
int HDecimator<int16_t>::WriteImpl(int16_t* src, size_t blocksize);

template
int HDecimator<int32_t>::WriteImpl(int32_t* src, size_t blocksize);

// Read()
template
int HDecimator<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HDecimator<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HDecimator<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HDecimator<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif
