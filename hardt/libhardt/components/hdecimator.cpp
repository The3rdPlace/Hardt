#ifndef __HDECIMATOR_CPP
#define __HDECIMATOR_CPP

#include "hdecimator.h"

template <class T>
HDecimator<T>::HDecimator(HWriter<T>* writer, int factor, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr)
{
    HLog("HDecimator(HWriter*, blocksize=%d)", blocksize);
    Init();
    
}

template <class T>
HDecimator<T>::HDecimator(HWriterConsumer<T>* consumer, int factor, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr)
{
    HLog("HDecimator(HWriterConsumer*, blocksize=%d)", blocksize);
    Init();
    consumer->SetWriter(this);
}

template <class T>
HDecimator<T>::HDecimator(HReader<T>* reader, int factor, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader)
{
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

    if( _blocksize % _factor != 0 ) {
        throw new HInitializationException("Decimation factor is not a valid divisor for the given blocksize");
    }

    _buffer = new T[_blocksize];
}

template <class T>
int HDecimator<T>::Write(T* src, size_t blocksize)
{
    if( _writer == nullptr )
    {
        throw new HInitializationException("This HDecimator is not a writer");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for write is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HWriterIOException("Requested blocksize for write is invalid");
    }

    // Decimate
    for(int i = 0; i < blocksize; i += _factor) {
        _buffer[_length++] = src[i];
    }
    if( _length == _blocksize ) {
        _writer->Write(_buffer, blocksize);
        _length = 0;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HDecimator<T>::Read(T* dest, size_t blocksize)
{
    if( _reader == nullptr )
    {
        throw new HInitializationException("This HDecimator is not a reader");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for read is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HReaderIOException("Requested blocksize for read is invalid");
    }

   
    // Update metrics
    _length = 0;
    while( _length < _blocksize ) {

        // Read a block
        int read = _reader->Read(_buffer, blocksize);
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
        for(int i = 0; i < blocksize; i += _factor) {
            dest[_length++] = _buffer[i];
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
HDecimator<int8_t>::HDecimator(HWriter<int8_t>* writer, int factor, size_t blocksize);

template
HDecimator<uint8_t>::HDecimator(HWriter<uint8_t>* writer, int factor, size_t blocksize);

template
HDecimator<int16_t>::HDecimator(HWriter<int16_t>* writer, int factor, size_t blocksize);

template
HDecimator<int32_t>::HDecimator(HWriter<int32_t>* writer, int factor, size_t blocksize);

template
HDecimator<int8_t>::HDecimator(HWriterConsumer<int8_t>* consumer, int factor, size_t blocksize);

template
HDecimator<uint8_t>::HDecimator(HWriterConsumer<uint8_t>* consumer, int factor, size_t blocksize);

template
HDecimator<int16_t>::HDecimator(HWriterConsumer<int16_t>* consumer, int factor, size_t blocksize);

template
HDecimator<int32_t>::HDecimator(HWriterConsumer<int32_t>* consumer, int factor, size_t blocksize);

template
HDecimator<int8_t>::HDecimator(HReader<int8_t>* reader, int factor, size_t blocksize);

template
HDecimator<uint8_t>::HDecimator(HReader<uint8_t>* reader, int factor, size_t blocksize);

template
HDecimator<int16_t>::HDecimator(HReader<int16_t>* reader, int factor, size_t blocksize);

template
HDecimator<int32_t>::HDecimator(HReader<int32_t>* reader, int factor, size_t blocksize);

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
int HDecimator<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HDecimator<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HDecimator<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HDecimator<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HDecimator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HDecimator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HDecimator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HDecimator<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
