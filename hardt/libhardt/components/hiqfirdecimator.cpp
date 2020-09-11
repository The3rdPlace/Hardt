#ifndef __HIQFIRDECIMATOR_CPP
#define __HIQFIRDECIMATOR_CPP

#include "hiqfirdecimator.h"

#include <cstring>

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr),
    _collect(collect)
{
    HLog("HIqFirDecimator(HWriter*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr),
    _collect(collect)
{
    HLog("HIqFirDecimator(HWriterConsumer*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
    consumer->SetWriter(this);
}

template <class T>
HIqFirDecimator<T>::HIqFirDecimator(HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader),
    _collect(collect)
{
    HLog("HIqFirDecimator(HReader*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HIqFirDecimator<T>::~HIqFirDecimator()
{
    delete[] _buffer;

    delete _firI;
    delete _firQ;

    delete _filteredI;
    delete _filteredQ;

    delete _I;
    delete _IOut;
    delete _Q;
    delete _QOut;
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

    _firI = new HFir<T>(coefficients, points, 1, _factor, 1);
    _firQ = new HFir<T>(coefficients, points, 1, _factor, 1);

    _filteredI = new T[_blocksize / 2];
    _filteredQ = new T[_blocksize / 2];

    _I = new T[_blocksize / 2];
    _IOut = new T[_blocksize / 2];
    _Q = new T[_blocksize / 2];
    _QOut = new T[_blocksize / 2];

}

template <class T>
int HIqFirDecimator<T>::Write(T* src, size_t blocksize)
{
    if( _writer == nullptr )
    {
        throw new HInitializationException("This HIqFirDecimator is not a writer");
    }

    if( blocksize != _blocksize )
    {
        HError("Requested blocksize for write is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HWriterIOException("Requested blocksize for write is invalid");
    }

    // Split multiplexed stream
    for( int i = 0; i < blocksize; i += 2 ) {
        _I[i / 2] = src[i];
        _Q[i / 2] = src[i + 1];
    }

    // Filter
    _firI->Filter(_I, _IOut, blocksize / 2);
    _firQ->Filter(_Q, _QOut, blocksize / 2);

    // Decimate
    for(int i = 0; i < blocksize / 2; i += _factor) {
        _filteredI[_length] = _IOut[i];
        _filteredQ[_length] = _QOut[i];
        _length++;
    }
    if( _length == _blocksize / 2 || !_collect) {

        // Combine multiplexed stream
        for( int i = 0; i < blocksize; i += 2 ) {
            _buffer[i] = _filteredI[i / 2];
            _buffer[i + 1] = _filteredQ[i / 2];
        }

        _writer->Write(_buffer, _length * 2);
        _length = 0;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HIqFirDecimator<T>::Read(T* dest, size_t blocksize)
{
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

        // Split multiplexed stream
        for( int i = 0; i < _blocksize; i += 2 ) {
            _I[i / 2] = _buffer[i];
            _Q[i / 2] = _buffer[i + 1];
        }

        // Filter
        _firI->Filter(_I, _IOut, _blocksize / 2);
        _firQ->Filter(_Q, _QOut, _blocksize / 2);

        // Decimate the block
        for(int i = 0; i < _blocksize / 2; i += _factor) {
            dest[_length++] = _IOut[i];
            dest[_length++] = _QOut[i];
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
HIqFirDecimator<int8_t>::HIqFirDecimator(HWriter<int8_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(HWriter<uint8_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(HWriter<int16_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(HWriter<int32_t>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int8_t>::HIqFirDecimator(HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int8_t>::HIqFirDecimator(HReader<int8_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<uint8_t>::HIqFirDecimator(HReader<uint8_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int16_t>::HIqFirDecimator(HReader<int16_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

template
HIqFirDecimator<int32_t>::HIqFirDecimator(HReader<int32_t>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect);

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
