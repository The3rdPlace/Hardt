#ifndef __HINTERPOLATOR_CPP
#define __HINTERPOLATOR_CPP

#include "hinterpolator.h"

template <class T>
HInterpolator<T>::HInterpolator(HWriter<T>* writer, int factor, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _writer(writer),
    _reader(nullptr),
    _coefficients(nullptr),
    _length(0) {

    HLog("HInterpolator(HWriter*, factor=%d, blocksize=%d)", factor, blocksize);
    Init();
    
}

template <class T>
HInterpolator<T>::HInterpolator(HWriter<T>* writer, int factor, float* coefficients, int length, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _writer(writer),
    _reader(nullptr),
    _coefficients(nullptr),
    _length(length) {

    HLog("HInterpolator(HWriter*, factor=%d, float*, length=%d, blocksize=%d)", factor, length, blocksize);
    Init(coefficients);
    
}

template <class T>
HInterpolator<T>::HInterpolator(HWriterConsumer<T>* consumer, int factor, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(nullptr),
    _coefficients(nullptr),
    _length(0) {

    HLog("HInterpolator(HWriterConsumer*, factor=%d, blocksize=%d)", factor, blocksize);
    Init();
    consumer->SetWriter(this);
}

template <class T>
HInterpolator<T>::HInterpolator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int length, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(nullptr),
    _coefficients(nullptr),
    _length(length) {

    HLog("HInterpolator(HWriterConsumer*, factor=%d, length=%d, blocksize=%d)", factor, length, blocksize);
    Init(coefficients);
    consumer->SetWriter(this);
}

template <class T>
HInterpolator<T>::HInterpolator(HReader<T>* reader, int factor, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(reader),
    _coefficients(nullptr),
    _length(0) {

    HLog("HInterpolator(HReader*, factor=%d, blocksize=%d)", factor, blocksize);
    Init();
}

template <class T>
HInterpolator<T>::HInterpolator(HReader<T>* reader, int factor, float* coefficients, int length, size_t blocksize):
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(reader),
    _coefficients(nullptr),
    _length(length) {
        
    HLog("HInterpolator(HReader*, factor=%d, length=%d, blocksize=%d)", factor, length, blocksize);
    Init(coefficients);
}

template <class T>
HInterpolator<T>::~HInterpolator()
{
    delete[] _buffer;
    if( _coefficients != nullptr ) {
        delete[] _coefficients;
    }
}

template <class T>
void HInterpolator<T>::Init(float* coefficients) {

    if( _factor <= 0 ) {
        throw new HInitializationException("Interpolation factor can not be zero or negative");
    }

    if( _blocksize % _factor != 0 ) {
        throw new HInitializationException("Interpolation factor is not a valid divisor for the given blocksize");
    }

    _buffer = new T[_blocksize * _factor];

    if( _length > 0 ) {

        // Rearrange coefficienets for multiple polyphase filters
        _coefficients = new float[_length];
        int k = 0;
        for( int i = 0; i < _factor; i++ ) {
            for( int j = 0; j < _length / _factor; j += factor ) {
                HLog("i=%d  j=%d  k=%d", i, j, k);
                _coefficients[k++] = coefficients[i + j];
            }
        }

        // Todo: Setup filters
    }
}

template <class T>
int HInterpolator<T>::Write(T* src, size_t blocksize)
{
    if( _writer == nullptr )
    {
        throw new HInitializationException("This HInterpolator is not a writer");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for write is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HWriterIOException("Requested blocksize for write is invalid");
    }

    // Upsample and/or filter
    int j = 0;
    memset((void*) _buffer, 0, blocksize * sizeof(T) * _factor);
    if( _length == 0 ) {  
        
        // No filtering = upsampling
        for( int i = 0; i < blocksize * _factor; i += _factor) {
            _buffer[i] = src[j++];
        }

    } else {

        // Interpolation

    }

    // Write
    ((HWriter<T>*) this)->Metrics.Writes++;    
    for( int i = 0; i < _factor; i++ ) {
        _writer->Write(&_buffer[i * blocksize], blocksize);
        ((HWriter<T>*) this)->Metrics.BlocksOut++;
        ((HWriter<T>*) this)->Metrics.BytesOut += sizeof(T) * blocksize;
    }

    // Always return the full input blocksize
    return blocksize;
}

template <class T>
int HInterpolator<T>::Read(T* dest, size_t blocksize)
{
    if( _reader == nullptr )
    {
        throw new HInitializationException("This HInterpolator is not a reader");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for read is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HReaderIOException("Requested blocksize for read is invalid");
    }

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
    ((HReader<T>*) this)->Metrics.BlocksIn++;
    ((HReader<T>*) this)->Metrics.BytesIn += sizeof(T) * blocksize;

    // Upsamle and/or filter
    int j = 0;
    memset((void*) dest, 0, blocksize * sizeof(T) * _factor);
    if( _length == 0 ) {  
        
        // No filtering = upsampling
        for( int i = 0; i < blocksize * _factor; i += _factor) {
            _dest[i] = _buffer[j++];

    } else {

        // Interpolation

    }

    // Return complete decimated read
    ((HReader<T>*) this)->Metrics.Reads++;    
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HInterpolator()
template
HInterpolator<int8_t>::HInterpolator(HWriter<int8_t>* writer, int factor, size_t blocksize);

template
HInterpolator<uint8_t>::HInterpolator(HWriter<uint8_t>* writer, int factor, size_t blocksize);

template
HInterpolator<int16_t>::HInterpolator(HWriter<int16_t>* writer, int factor, size_t blocksize);

template
HInterpolator<int32_t>::HInterpolator(HWriter<int32_t>* writer, int factor, size_t blocksize);

template
HInterpolator<int8_t>::HInterpolator(HWriter<int8_t>* writer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<uint8_t>::HInterpolator(HWriter<uint8_t>* writer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int16_t>::HInterpolator(HWriter<int16_t>* writer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int32_t>::HInterpolator(HWriter<int32_t>* writer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int8_t>::HInterpolator(HWriterConsumer<int8_t>* consumer, int factor, size_t blocksize);

template
HInterpolator<uint8_t>::HInterpolator(HWriterConsumer<uint8_t>* consumer, int factor, size_t blocksize);

template
HInterpolator<int16_t>::HInterpolator(HWriterConsumer<int16_t>* consumer, int factor, size_t blocksize);

template
HInterpolator<int32_t>::HInterpolator(HWriterConsumer<int32_t>* consumer, int factor, size_t blocksize);

template
HInterpolator<int8_t>::HInterpolator(HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<uint8_t>::HInterpolator(HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int16_t>::HInterpolator(HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int32_t>::HInterpolator(HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int8_t>::HInterpolator(HReader<int8_t>* reader, int factor, size_t blocksize);

template
HInterpolator<uint8_t>::HInterpolator(HReader<uint8_t>* reader, int factor, size_t blocksize);

template
HInterpolator<int16_t>::HInterpolator(HReader<int16_t>* reader, int factor, size_t blocksize);

template
HInterpolator<int32_t>::HInterpolator(HReader<int32_t>* reader, int factor, size_t blocksize);

template
HInterpolator<int8_t>::HInterpolator(HReader<int8_t>* reader, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<uint8_t>::HInterpolator(HReader<uint8_t>* reader, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int16_t>::HInterpolator(HReader<int16_t>* reader, int factor, float* coefficients, int length, size_t blocksize);

template
HInterpolator<int32_t>::HInterpolator(HReader<int32_t>* reader, int factor, float* coefficients, int length, size_t blocksize);

// ~HInterpolator()
template
HInterpolator<int8_t>::~HInterpolator();

template
HInterpolator<uint8_t>::~HInterpolator();

template
HInterpolator<int16_t>::~HInterpolator();

template
HInterpolator<int32_t>::~HInterpolator();

// Init()
template
void HInterpolator<int8_t>::Init(float* coefficients);

template
void HInterpolator<uint8_t>::Init(float* coefficients);

template
void HInterpolator<int16_t>::Init(float* coefficients);

template
void HInterpolator<int32_t>::Init(float* coefficients);

// Write()
template
int HInterpolator<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HInterpolator<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HInterpolator<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HInterpolator<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HInterpolator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HInterpolator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HInterpolator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HInterpolator<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
