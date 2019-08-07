#ifndef __HFIRFILTER_CPP
#define __HFIRFILTER_CPP

#include "hfirfilter.h"

template <class T>
HFirFilter<T>::HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(writer, blocksize),
    _length(length),
    _firstLength(length)
{
    HLog("HFirFilter(HWriter*)");
    Init(coefficients, length);
}

template <class T>
HFirFilter<T>::HFirFilter(HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(consumer, blocksize),
    _length(length),
    _firstLength(length)
{
    HLog("HFirFilter(HWriterConsumer*)");
    Init(coefficients, length);

    consumer->SetWriter(this);
}

template <class T>
HFirFilter<T>::HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    HFilter<T>(reader, blocksize),
    _length(length),
    _firstLength(length)
{
    HLog("HFirFilter(HReader*)");
    Init(coefficients, length);
}

template <class T>
void HFirFilter<T>::Init(float* coefficients, int length)
{
    HLog("Init(float*, %d)", length);

    _coefficients = new float[length];
    SetCoefficients(coefficients, length);
    HLog("Copied filter coefficients");

    _taps = new T[length];
    for( int i = 0; i < length; i++ )
    {
        _taps[i] = 0;
    }
    HLog("Allocated and initialized delay buffer for %d taps", length);
}

template <class T>
HFirFilter<T>::~HFirFilter()
{
    HLog("~HFirFilter()");
    delete[] _taps;
    delete[] _coefficients;
}

template <class T>
void HFirFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Run FIR filter
    for( int i = 0; i < blocksize; i++ )
    {
        // Advance delay line 1 sample
        memcpy((void*) &_taps[1], (void*) _taps, (_length - 1) * sizeof(T));

        // Add new sample
        _taps[0] = src[i];

        // Sum all taps
        float result = 0;
        for( int j = 0; j < _length; j++ )
        {
            result += _taps[j] * _coefficients[j];
        }

        // Store result for 1 sample
        dest[i] = result;
    }
}

template <class T>
void HFirFilter<T>::SetCoefficients(float* coefficients, int length)
{
    // Sanity check
    if( length != _firstLength )
    {
        HError("It is not possible to assign a set of coefficients of different length than the length used at construction (%d)", _firstLength);
        throw new HFilterInitializationException("Length of coefficients differs from construction length");
    }

    // Copy coefficients
    memcpy(_coefficients, coefficients, length * sizeof(float));
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HFirFilter
template
HFirFilter<int8_t>::HFirFilter(HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int8_t>::HFirFilter(HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int8_t>::HFirFilter(HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

// Init
template
void HFirFilter<int8_t>::Init(float* coefficients, int length);

template
void HFirFilter<uint8_t>::Init(float* coefficients, int length);

template
void HFirFilter<int16_t>::Init(float* coefficients, int length);

template
void HFirFilter<int32_t>::Init(float* coefficients, int length);

// ~HFirFilter()
template
HFirFilter<int8_t>::~HFirFilter();

template
HFirFilter<uint8_t>::~HFirFilter();

template
HFirFilter<int16_t>::~HFirFilter();

template
HFirFilter<int32_t>::~HFirFilter();

// Filter
template
void HFirFilter<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HFirFilter<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HFirFilter<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HFirFilter<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// SetCoefficients
template
void HFirFilter<int8_t>::SetCoefficients(float* coefficients, int length);

template
void HFirFilter<uint8_t>::SetCoefficients(float* coefficients, int length);

template
void HFirFilter<int16_t>::SetCoefficients(float* coefficients, int length);

template
void HFirFilter<int32_t>::SetCoefficients(float* coefficients, int length);

#endif