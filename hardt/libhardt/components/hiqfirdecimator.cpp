#ifndef __HIQFIRDECIMATOR_CPP
#define __HIQFIRDECIMATOR_CPP

#include "hiqfirdecimator.h"

#include <cstring>

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(std::string id, HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr),
    _collect(collect) {
    HLog("HIqFirDecimator(HWriter*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(std::string id, HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr),
    _collect(collect) {
    HLog("HIqFirDecimator(HWriterConsumer*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
    consumer->SetWriter(this);
}

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(std::string id, HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader),
    _collect(collect) {
    HLog("HIqFirDecimator(HReader*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HIqFirDecimator<T>::~HIqFirDecimator() {

    delete[] _buffer;
    delete[] _filtered;

    delete _firI;
    delete _firQ;
}

template <class T>
void HIqFirDecimator<T>::Init(float* coefficients, int points) {

    if( _factor <= 0 ) {
        throw new HInitializationException("Decimation factor can not be zero or negative");
    }

    if( _blocksize % _factor != 0 ) {
        throw new HInitializationException("Decimation factor is not a valid divisor for the given blocksize");
    }

    _buffer = new T[_blocksize];
    _filtered = new T[_blocksize];

    _firI = new HFir<T>(coefficients, points, 1, _factor, 2);
    _firQ = new HFir<T>(coefficients, points, 1, _factor, 2);
}

template <class T>
int HIqFirDecimator<T>::WriteImpl(T* src, size_t blocksize) {

    if( _writer == nullptr )
    {
        throw new HInitializationException("This HIqFirDecimator is not a writer");
    }

    if( blocksize != _blocksize )
    {
        HError("Requested blocksize for write is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HWriterIOException("Requested blocksize for write is invalid");
    }

    // Filter
    _firI->Filter(src, _filtered, blocksize);
    _firQ->Filter(&src[1], &_filtered[1], blocksize);

    // Decimate
    for(int i = 0; i < blocksize; i += _factor * 2) {
        _buffer[_length++] = _filtered[i];
        _buffer[_length++] = _filtered[i + 1];
    }

    // Write result if we have reached blocksize (or not collecting)
    if( _length == _blocksize || !_collect) {
        _writer->Write(_buffer, _length);
        _length = 0;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HIqFirDecimator<T>::ReadImpl(T* dest, size_t blocksize) {

    if( _reader == nullptr )
    {
        throw new HInitializationException("This HIqFirDecimator is not a reader");
    }

    if( _collect ) {
        if (blocksize != _blocksize) {
            HError("Requested blocksize for collected read is invalid: %d requested, expected is %d", blocksize, _blocksize);
            throw new HReaderIOException("Requested blocksize for read is invalid");
        }
    } else {
        if (blocksize != _blocksize / _factor) {
            HError("Requested blocksize for read is invalid: %d requested, expected is %d", blocksize, _blocksize / _factor);
            throw new HReaderIOException("Requested blocksize for read is invalid");
        }
    }
   
    // Read enough blocks to return a full decimated buffer
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

        // Filter
        _firI->Filter(_buffer, _filtered, _blocksize);
        _firQ->Filter(&_buffer[1], &_filtered[1], _blocksize);

        // Decimate the block
        for( int i = 0; i < _blocksize; i += _factor * 2) {
            dest[_length++] = _filtered[i];
            dest[_length++] = _filtered[i + 1];
        }

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

// HIqFirDecimator()
template
HIqFirDecimator<int8_t>::HIqFirDecimator(std::string id, HWriter<int8_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(std::string id, HWriter<uint8_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(std::string id, HWriter<int16_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(std::string id, HWriter<int32_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int8_t>::HIqFirDecimator(std::string id, HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(std::string id, HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(std::string id, HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(std::string id, HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int8_t>::HIqFirDecimator(std::string id, HReader<int8_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(std::string id, HReader<uint8_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(std::string id, HReader<int16_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(std::string id, HReader<int32_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

// ~HIqFirDecimator()
template
HIqFirDecimator<int8_t>::~HIqFirDecimator();

template
HIqFirDecimator<uint8_t>::~HIqFirDecimator();

template
HIqFirDecimator<int16_t>::~HIqFirDecimator();

template
HIqFirDecimator<int32_t>::~HIqFirDecimator();

// Init()
template
void HIqFirDecimator<int8_t>::Init(float* coefficients, int points);

template
void HIqFirDecimator<uint8_t>::Init(float* coefficients, int points);

template
void HIqFirDecimator<int16_t>::Init(float* coefficients, int points);

template
void HIqFirDecimator<int32_t>::Init(float* coefficients, int points);

// Write()
template
int HIqFirDecimator<int8_t>::WriteImpl(int8_t* src, size_t blocksize);

template
int HIqFirDecimator<uint8_t>::WriteImpl(uint8_t* src, size_t blocksize);

template
int HIqFirDecimator<int16_t>::WriteImpl(int16_t* src, size_t blocksize);

template
int HIqFirDecimator<int32_t>::WriteImpl(int32_t* src, size_t blocksize);

// Read()
template
int HIqFirDecimator<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HIqFirDecimator<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HIqFirDecimator<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HIqFirDecimator<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif
