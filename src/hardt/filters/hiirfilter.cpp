#ifndef __HIIRFILTER_CPP
#define __HIIRFILTER_CPP

#include "hiirfilter.h"

template <class T>
HIirFilter<T>::HIirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(writer, blocksize),
    _length(length / 2)
{
    HLog("HIirFilter(HWriter*)");
    Init(coefficients, length);
}

template <class T>
HIirFilter<T>::HIirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    HFilter<T>(reader, blocksize),
    _length(length / 2)
{
    HLog("HIirFilter(HReader*)");
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
    _aCoefficients = new float[_length];
    _bCoefficients = new float[_length + 1];
    memcpy(_bCoefficients, coefficients, (_length + 1) * sizeof(float));
    memcpy(_aCoefficients, &coefficients[_length + 1], _length * sizeof(float));
    HLog("Copied filter coefficients");

    _taps = new T[_length + 1];
    _output = new float[_length + 1];
    for( int i = 0; i < _length + 1; i++ )
    {
        _taps[i] = 0;
        _output[i] = 0;
    }
    HLog("Allocated and initialized delay buffers for %d taps and output values", _length + 1);
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
        memcpy((void*) &_taps[1], (void*) _taps, (_length + 1) * sizeof(T));
        memcpy((void*) &_output[1], (void*) _output, (_length + 1) * sizeof(float));

        // Add new sample
        _taps[0] = src[i];

        // Sum all taps
        float result = 0;
        result += _taps[0] * _bCoefficients[0];
        for( int j = 1; j < _length + 1; j++ )
        {
            result += _taps[j] * _bCoefficients[j];
            result -= _output[j] * _aCoefficients[j - 1];
        }

        // Add output to the feedback delay line
        _output[0] = result;

        // Store result for 1 sample
        dest[i] = result;
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HIirFilter
template
HIirFilter<int8_t>::HIirFilter(HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<uint8_t>::HIirFilter(HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int16_t>::HIirFilter(HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int32_t>::HIirFilter(HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int8_t>::HIirFilter(HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIirFilter<uint8_t>::HIirFilter(HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int16_t>::HIirFilter(HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIirFilter<int32_t>::HIirFilter(HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

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

#endif