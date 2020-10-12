#ifndef __HIQFIRDECIMATOR_CPP
#define __HIQFIRDECIMATOR_CPP

#include "hiqfirdecimator.h"

#include <cstring>

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<T>* probe):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr),
    _collect(collect),
    _probe(probe) {

    HLog("HIqFirDecimator(HWriter*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<T>* probe):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr),
    _collect(collect),
    _probe(probe) {

    HLog("HIqFirDecimator(HWriterConsumer*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
    consumer->SetWriter(this);
}

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<T>* probe):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader),
    _collect(collect),
    _probe(probe) {

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
int HIqFirDecimator<T>::Write(T* src, size_t blocksize) {

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
        if( _probe != nullptr ) {
            _probe->Write(_buffer, _length);
        }
        _length = 0;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HIqFirDecimator<T>::Read(T* dest, size_t blocksize) {

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
            if( _probe != nullptr ) {
                _probe->Write(dest, _length);
            }
            break;
        }
    }

    // If collecting, write to probe - if set
    if( _collect && _probe != nullptr ) {
        _probe->Write(dest, _length);
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
HIqFirDecimator<int8_t>::HIqFirDecimator(HWriter<int8_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int8_t>* probe);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(HWriter<uint8_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<uint8_t>* probe);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(HWriter<int16_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int16_t>* probe);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(HWriter<int32_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int32_t>* probe);

template
HIqFirDecimator<int8_t>::HIqFirDecimator(HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int8_t>* probe);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<uint8_t>* probe);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int16_t>* probe);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int32_t>* probe);

template
HIqFirDecimator<int8_t>::HIqFirDecimator(HReader<int8_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int8_t>* probe);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(HReader<uint8_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<uint8_t>* probe);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(HReader<int16_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int16_t>* probe);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(HReader<int32_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect, HProbe<int32_t>* probe);

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
int HIqFirDecimator<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HIqFirDecimator<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HIqFirDecimator<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HIqFirDecimator<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HIqFirDecimator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HIqFirDecimator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HIqFirDecimator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HIqFirDecimator<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
