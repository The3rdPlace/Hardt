#ifndef __HINTERPOLATOR_CPP
#define __HINTERPOLATOR_CPP

#include "hinterpolator.h"

#include <vector>

template <class T>
HInterpolator<T>::HInterpolator(std::string id, HWriter<T>* writer, int factor, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _writer(writer),
    _reader(nullptr),
    _length(0),
    _coefficients(nullptr),
    _firLength(0),
    _probe(probe) {

    HLog("HInterpolator(HWriter*, factor=%d, blocksize=%d)", factor, blocksize);
    Init();
}

template <class T>
HInterpolator<T>::HInterpolator(std::string id, HWriter<T>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _writer(writer),
    _reader(nullptr),
    _length(0),
    _coefficients(nullptr),
    _firLength(length),
    _probe(probe) {

    HLog("HInterpolator(HWriter*, factor=%d, float*, length=%d, blocksize=%d)", factor, length, blocksize);
    Init(coefficients);
}

template <class T>
HInterpolator<T>::HInterpolator(std::string id, HWriterConsumer<T>* consumer, int factor, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(nullptr),
    _length(0),
    _coefficients(nullptr),
    _firLength(0),
    _probe(probe) {

    HLog("HInterpolator(HWriterConsumer*, factor=%d, blocksize=%d)", factor, blocksize);
    Init();
    consumer->SetWriter(this);
}

template <class T>
HInterpolator<T>::HInterpolator(std::string id, HWriterConsumer<T>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(nullptr),
    _length(0),
    _coefficients(nullptr),
    _firLength(length),
    _probe(probe) {

    HLog("HInterpolator(HWriterConsumer*, factor=%d, length=%d, blocksize=%d)", factor, length, blocksize);
    Init(coefficients);
    consumer->SetWriter(this);
}

template <class T>
HInterpolator<T>::HInterpolator(std::string id, HReader<T>* reader, int factor, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(reader),
    _length(0),
    _coefficients(nullptr),
    _firLength(0),
    _probe(probe) {

    HLog("HInterpolator(HReader*, factor=%d, blocksize=%d)", factor, blocksize);
    Init();
}

template <class T>
HInterpolator<T>::HInterpolator(std::string id, HReader<T>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _blocksize(blocksize),
    _factor(factor),
    _writer(nullptr),
    _reader(reader),
    _length(0),
    _coefficients(nullptr),
    _firLength(length),
    _probe(probe) {
        
    HLog("HInterpolator(HReader*, factor=%d, length=%d, blocksize=%d)", factor, length, blocksize);
    Init(coefficients);
}

template <class T>
HInterpolator<T>::~HInterpolator()
{
    delete[] _inBuffer;
    delete[] _outBuffer;

    if( _coefficients != nullptr ) {
        delete[] _coefficients;
    }

    if( _firLength > 0 ) {
        for( typename std::vector< HFir<T>* >::iterator it = _firs.begin(); it != _firs.end(); it++ ) {
            delete *it;
        }
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

    if( _firLength > 0 ) {

        // Rearrange coefficients for multiple polyphase filters
        _coefficients = new float[_firLength];
        int k = 0;
        for( int i = 0; i < _factor; i++ ) {
            for( int j = 0; j < _firLength; j += _factor ) {
                _coefficients[k++] = coefficients[i + j];
            }
        }

        // Setup fir filter with rearranged coefficients
        for( int i = 0; i < _factor; i++ ) {
            _firs.push_back( new HFir<T>(&_coefficients[ i * _firLength / _factor ], _firLength / _factor, _factor, 1, 1) );
        }
    }

    // Temporary buffer for results
    _inBuffer = new T[_blocksize];
    _outBuffer = new T[_blocksize * _factor];
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
    if( _firLength == 0 ) {

        // No filtering = upsampling
        memset((void*) _outBuffer, 0, blocksize * sizeof(T) * _factor);
        for( int i = 0; i < blocksize * _factor; i += _factor) {
            _outBuffer[i] = src[i / _factor];
        }

    } else {

        // Interpolation: upsample and filter in one go using '_factor' polyphase FIR filters
        int i = 0;
        for( typename std::vector< HFir<T>* >::iterator it = _firs.begin(); it != _firs.end(); it++ ) {
            (*it)->Filter(src, &_outBuffer[i++], blocksize);
        }
    }

    // Write
    for( int i = 0; i < _factor; i++ ) {
        _writer->Write(&_outBuffer[i * blocksize], blocksize);
    }

    // Output to probe ?
    if( _probe != nullptr ) {
        _probe->Write(_outBuffer, blocksize * _factor);
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

    if( blocksize != _blocksize )
    {
        HError("Requested blocksize for read is invalid: %d requested, expected is %d", blocksize, _blocksize);
        throw new HReaderIOException("Requested blocksize for read is invalid");
    }

    // Already have data available ?
    if( _length > 0 ) {
        memcpy((void*) dest, (void*) &_outBuffer[_length], blocksize);
        _length += blocksize;

        if( _length == _blocksize * _factor ) {
            _length = 0;
        }

        return blocksize;
    }

    // Read a block
    int read = _reader->Read(_inBuffer, blocksize);
    if( read == 0 ) {
        HLog("Received zero-read, returning zero");
        return 0;
    }
    else if( read < _blocksize ) {
        HLog("Received too low read size, inserting silence");
        memset((void*) &_inBuffer[read], 0, _blocksize - read);

    }
    else if( read > _blocksize ) {
        HError("Received too big read size, memory corruption must be expected. Stopping");
        return 0;
    }

    // Upsampling or interpolation ?
    if( _firLength == 0 ) {

        // No filtering = upsampling
        memset((void*) _outBuffer, 0, blocksize * sizeof(T) * _factor);
        for( int i = 0; i < blocksize * _factor; i += _factor) {
            _outBuffer[i] = _inBuffer[i / _factor];
        }

    } else {

        // Interpolation: upsample and filter in one go using '_factor' polyphase FIR filters
        int i = 0;
        for( typename std::vector< HFir<T>* >::iterator it = _firs.begin(); it != _firs.end(); it++ ) {
            (*it)->Filter(_inBuffer, &_outBuffer[i++], blocksize);
        }
    }

    // Return first part of decimated read
    memcpy((void*) dest, (void*) &_outBuffer[0], blocksize);
    _length += blocksize;    
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HInterpolator()
template
HInterpolator<int8_t>::HInterpolator(std::string id, HWriter<int8_t>* writer, int factor, size_t blocksize, HProbe<int8_t>* probe);

template
HInterpolator<uint8_t>::HInterpolator(std::string id, HWriter<uint8_t>* writer, int factor, size_t blocksize, HProbe<uint8_t >* probe);

template
HInterpolator<int16_t>::HInterpolator(std::string id, HWriter<int16_t>* writer, int factor, size_t blocksize, HProbe<int16_t>* probe);

template
HInterpolator<int32_t>::HInterpolator(std::string id, HWriter<int32_t>* writer, int factor, size_t blocksize, HProbe<int32_t>* probe);

template
HInterpolator<int8_t>::HInterpolator(std::string id, HWriter<int8_t>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HInterpolator<uint8_t>::HInterpolator(std::string id, HWriter<uint8_t>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HInterpolator<int16_t>::HInterpolator(std::string id, HWriter<int16_t>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HInterpolator<int32_t>::HInterpolator(std::string id, HWriter<int32_t>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HInterpolator<int8_t>::HInterpolator(std::string id, HWriterConsumer<int8_t>* consumer, int factor, size_t blocksize, HProbe<int8_t>* probe);

template
HInterpolator<uint8_t>::HInterpolator(std::string id, HWriterConsumer<uint8_t>* consumer, int factor, size_t blocksize, HProbe<uint8_t>* probe);

template
HInterpolator<int16_t>::HInterpolator(std::string id, HWriterConsumer<int16_t>* consumer, int factor, size_t blocksize, HProbe<int16_t>* probe);

template
HInterpolator<int32_t>::HInterpolator(std::string id, HWriterConsumer<int32_t>* consumer, int factor, size_t blocksize, HProbe<int32_t>* probe);

template
HInterpolator<int8_t>::HInterpolator(std::string id, HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HInterpolator<uint8_t>::HInterpolator(std::string id, HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HInterpolator<int16_t>::HInterpolator(std::string id, HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HInterpolator<int32_t>::HInterpolator(std::string id, HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HInterpolator<int8_t>::HInterpolator(std::string id, HReader<int8_t>* reader, int factor, size_t blocksize, HProbe<int8_t>* probe);

template
HInterpolator<uint8_t>::HInterpolator(std::string id, HReader<uint8_t>* reader, int factor, size_t blocksize, HProbe<uint8_t>* probe);

template
HInterpolator<int16_t>::HInterpolator(std::string id, HReader<int16_t>* reader, int factor, size_t blocksize, HProbe<int16_t>* probe);

template
HInterpolator<int32_t>::HInterpolator(std::string id, HReader<int32_t>* reader, int factor, size_t blocksize, HProbe<int32_t>* probe);

template
HInterpolator<int8_t>::HInterpolator(std::string id, HReader<int8_t>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HInterpolator<uint8_t>::HInterpolator(std::string id, HReader<uint8_t>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HInterpolator<int16_t>::HInterpolator(std::string id, HReader<int16_t>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HInterpolator<int32_t>::HInterpolator(std::string id, HReader<int32_t>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

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
