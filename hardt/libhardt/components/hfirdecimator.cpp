#ifndef __HFIRDECIMATOR_CPP
#define __HFIRDECIMATOR_CPP

#include "hfirdecimator.h"

template <class T>
HFirDecimator<T>::HFirDecimator(HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(writer),
    _reader(nullptr)
{
    HLog("HFirDecimator(HWriter*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HFirDecimator<T>::HFirDecimator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(nullptr)
{
    HLog("HFirDecimator(HWriterConsumer*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
    consumer->SetWriter(this);
}

template <class T>
HFirDecimator<T>::HFirDecimator(HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _length(0),
    _writer(nullptr),
    _reader(reader)
{
    HLog("HFirDecimator(HReader*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients, points);
}

template <class T>
HFirDecimator<T>::~HFirDecimator()
{
    delete[] _buffer;

    delete _fir;
    delete _filtered;
}

template <class T>
void HFirDecimator<T>::Init(float* coefficients, int points) {

    if( _factor <= 0 ) {
        throw new HInitializationException("Decimation factor can not be zero or negative");
    }

    if( _blocksize % _factor != 0 ) {
        throw new HInitializationException("Decimation factor is not a valid divisor for the given blocksize");
    }

    _buffer = new T[_blocksize];

    _fir = new HFir<T>(coefficients, points, 1, _factor);
    _filtered = new T[_blocksize];
}

template <class T>
int HFirDecimator<T>::Write(T* src, size_t blocksize)
{
    if( _writer == nullptr )
    {
        throw new HInitializationException("This HFirDecimator is not a writer");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for write is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HWriterIOException("Requested blocksize for write is invalid");
    }

    // Filter
    _fir->Filter(src, _filtered, blocksize);

    // Decimate
    for(int i = 0; i < blocksize; i += _factor) {
        _buffer[_length++] = _filtered[i];
    }
    if( _length == _blocksize ) {
        _writer->Write(_buffer, blocksize);
        _length = 0;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HFirDecimator<T>::Read(T* dest, size_t blocksize)
{
    if( _reader == nullptr )
    {
        throw new HInitializationException("This HFirDecimator is not a reader");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for read is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HReaderIOException("Requested blocksize for read is invalid");
    }
   
    // Read enough blocks to return a full decimated buffer
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

        // Filter
        _fir->Filter(_buffer, _filtered, blocksize);

        // Decimate the block
        for(int i = 0; i < blocksize; i += _factor) {
            dest[_length++] = _filtered[i];
        }
    }

    // Return complete decimated read
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HFirDecimator()
template
HFirDecimator<int8_t>::HFirDecimator(HWriter<int8_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HWriter<uint8_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HWriter<int16_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HWriter<int32_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int8_t>::HFirDecimator(HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int8_t>::HFirDecimator(HReader<int8_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HReader<uint8_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HReader<int16_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HReader<int32_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

// ~HFirDecimator()
template
HFirDecimator<int8_t>::~HFirDecimator();

template
HFirDecimator<uint8_t>::~HFirDecimator();

template
HFirDecimator<int16_t>::~HFirDecimator();

template
HFirDecimator<int32_t>::~HFirDecimator();

// Init()
template
void HFirDecimator<int8_t>::Init(float* coefficients, int points);

template
void HFirDecimator<uint8_t>::Init(float* coefficients, int points);

template
void HFirDecimator<int16_t>::Init(float* coefficients, int points);

template
void HFirDecimator<int32_t>::Init(float* coefficients, int points);

// Write()
template
int HFirDecimator<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HFirDecimator<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HFirDecimator<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HFirDecimator<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HFirDecimator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HFirDecimator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HFirDecimator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HFirDecimator<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
