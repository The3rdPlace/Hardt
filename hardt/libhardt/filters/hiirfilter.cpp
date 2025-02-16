#ifndef __HIIRFILTER_CPP
#define __HIIRFILTER_CPP

#include "hiirfilter.h"

template <class T>
HIirFilter<T>::HIirFilter(std::string id, HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, writer, blocksize),
    _length(length / 2),
    _firstLength(length)
{
    HLog("HIirFilter(HWriter*)");
    HLog("_length = %d, _firstLength = %d", _length, _firstLength);
    Init(coefficients, length);
}

template <class T>
HIirFilter<T>::HIirFilter(std::string id, HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, consumer, blocksize),
    _length(length / 2),
    _firstLength(length)
{
    HLog("HIirFilter(HWriterConsumer*)");
    HLog("_length = %d, _firstLength = %d", _length, _firstLength);
    Init(coefficients, length);

    consumer->SetWriter(this);
}

template <class T>
HIirFilter<T>::HIirFilter(std::string id, HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, reader, blocksize),
    _length(length / 2),
    _firstLength(length)
{
    HLog("HIirFilter(HReader*)");
    HLog("_length = %d, _firstLength = %d", _length, _firstLength);
    Init(coefficients, length);
}

template <class T>
void HIirFilter<T>::Init(float* coefficients, int length)
{
    HLog("Init(float*, %d)", length);

    // Expect an unequal number of coefficients to proper implement (at least)
    // a single normalized biquad
    if( length % 2 == 0 || length < 3 )
    {
        HError("An IIR filter needs an unequal number of a and b coefficients, ex. 3 b and 2 a");
        throw new HFilterInitializationException("Not enough coefficients");
    }

    // When receiving the coefficients array, we expect the b0,...,bN coefficients to come first.
    // It seems that all designers returns coefficienets this way.
    // Also be aware that we expect the input a1 and a2 coefficient to have the '-1' multiplied
    // already, so that the filter need only do a summation !!
    _aCoefficients = new float[_length];
    _bCoefficients = new float[_length + 1];
    SetCoefficients(coefficients, length);
    HLog("Copied filter coefficients");

    _taps = new T[_length + 1];
    _output = new float[_length + 1];
    for( int i = 0; i < _length + 1; i++ )
    {
        _taps[i] = 0;
        _output[i] = 0;
    }
    HLog("Allocated and initialized delay buffers for %d taps and output values", _length + 1);


    for( int i = 0; i < _length + 1; i++ )
    {
        HLog("b%d = %f", i + 1, _bCoefficients[i]);
    }
    for( int i = 0; i < _length; i++ )
    {
        HLog("a%d = %f", i + 1, _aCoefficients[i]);
    }
}

template <class T>
HIirFilter<T>::~HIirFilter()
{
    HLog("~HIirFilter()");
    delete[] _taps;
    delete[] _output;
    delete[] _aCoefficients;
    delete[] _bCoefficients;
}

template <class T>
void HIirFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Run IIR filter
    for( int i = 0; i < blocksize; i++ )
    {
        // Advance delay lines 1 sample
        memmove((void*) &_taps[1], (void*) _taps, (_length ) * sizeof(T));
        memmove((void*) &_output[1], (void*) _output, (_length ) * sizeof(float));

        // Add new sample
        _taps[0] = src[i];

        #ifdef DEEP_DEBUG
        for( int tap = 0; tap < _length + 1; tap++ )
        {
            HLog("_taps[%d]: %d", tap, _taps[tap]);
            HLog("_output[%d]: %f", tap, _output[tap]);
        }
        #endif

        // Sum all taps
        float result = 0;
        result += ((float) _taps[0]) * _bCoefficients[0];
        #ifdef DEEP_DEBUG
        HLog("_taps[0] * b0 = %f", result);
        #endif
        for( int j = 1; j < _length + 1; j++ )
        {
            result += ((float) _taps[j]) * _bCoefficients[j];
            #ifdef DEEP_DEBUG
            HLog("result + _taps[%d] * b%d = %f", j, j, result);
            #endif
            result += _output[j] * _aCoefficients[j - 1];
            #ifdef DEEP_DEBUG
            HLog("result + _output[%d] * a%d = %f", j, j, result);
            #endif
        }

        // Add output to the feedback delay line
        _output[0] = result;

        // Store result for 1 sample
        dest[i] = (T) result;
        #ifdef DEEP_DEBUG
        HLog("dest[%d] = %d", i, (T) result);
        #endif
    }
}

template <class T>
void HIirFilter<T>::SetCoefficients(float* coefficients, int length)
{
    // Sanity check
    if( length != _firstLength )
    {
        HError("It is not possible to assign a set of coefficients of different length than the length used at construction (%d)", _firstLength);
        throw new HFilterInitializationException("Length of coefficients differs from construction length");
    }

    // Copy coefficients
    memcpy(_bCoefficients, coefficients, (_length + 1) * sizeof(float));
    memcpy(_aCoefficients, &coefficients[_length + 1], _length * sizeof(float));
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HIirFilter
template
HIirFilter<int8_t>::HIirFilter(std::string id, HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<uint8_t>::HIirFilter(std::string id, HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int16_t>::HIirFilter(std::string id, HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int32_t>::HIirFilter(std::string id, HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int8_t>::HIirFilter(std::string id, HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<uint8_t>::HIirFilter(std::string id, HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int16_t>::HIirFilter(std::string id, HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int32_t>::HIirFilter(std::string id, HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int8_t>::HIirFilter(std::string id, HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIirFilter<uint8_t>::HIirFilter(std::string id, HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int16_t>::HIirFilter(std::string id, HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int32_t>::HIirFilter(std::string id, HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

// Init
template
void HIirFilter<int8_t>::Init(float* coefficients, int length);

template
void HIirFilter<uint8_t>::Init(float* coefficients, int length);

template
void HIirFilter<int16_t>::Init(float* coefficients, int length);

template
void HIirFilter<int32_t>::Init(float* coefficients, int length);

// ~HIirFilter()
template
HIirFilter<int8_t>::~HIirFilter();

template
HIirFilter<uint8_t>::~HIirFilter();

template
HIirFilter<int16_t>::~HIirFilter();

template
HIirFilter<int32_t>::~HIirFilter();

// Filter
template
void HIirFilter<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HIirFilter<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HIirFilter<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HIirFilter<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// SetCoefficients
template
void HIirFilter<int8_t>::SetCoefficients(float* coefficients, int length);

template
void HIirFilter<uint8_t>::SetCoefficients(float* coefficients, int length);

template
void HIirFilter<int16_t>::SetCoefficients(float* coefficients, int length);

template
void HIirFilter<int32_t>::SetCoefficients(float* coefficients, int length);

//! @endcond
#endif
