#ifndef __HFIRFILTER_CPP
#define __HFIRFILTER_CPP

#include "hfirfilter.h"

template <class T>
HFirFilter<T>::HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(writer, blocksize),
    _coefficients(coefficients),
    _length(length)
{
    HLog("HFirFilter(HWriter*)");

    _taps = new T[length];
    HLog("Allocated delay buffer for %d taps", length);
}

template <class T>
HFirFilter<T>::HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    HFilter<T>(reader, blocksize),
    _coefficients(coefficients),
    _length(length)
{
    HLog("HFirFilter(HReader*)");

    _taps = new T[length];
    HLog("Allocated delay buffer for %d taps", length);
}

template <class T>
HFirFilter<T>::~HFirFilter()
{
    HLog("~HFirFilter()");
    delete[] _taps;
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
HFirFilter<int8_t>::HFirFilter(HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

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

#endif