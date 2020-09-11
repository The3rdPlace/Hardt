#ifndef __HIQFIRFILTER_CPP
#define __HIQFIRFILTER_CPP

#include "hiqfirfilter.h"

template <class T>
HIqFirFilter<T>::HIqFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(writer, blocksize, probe) {

    HLog("HIqFirFilter(HWriter*)");
    HLog("_length = %d", length);
    Init(coefficients, length, blocksize);
}

template <class T>
HIqFirFilter<T>::HIqFirFilter(HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(consumer, blocksize, probe) {

    HLog("HIqFirFilter(HWriterConsumer*)");
    HLog("_length = %d", length);
    Init(coefficients, length, blocksize);

    consumer->SetWriter(this);
}

template <class T>
HIqFirFilter<T>::HIqFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(reader, blocksize, probe) {

    HLog("HIqFirFilter(HReader*)");
    HLog("_length = %d", length);
    Init(coefficients, length, blocksize);
}

template <class T>
void HIqFirFilter<T>::Init(float* coefficients, int length, size_t blocksize)
{
    HLog("Init(float*, %d)", length);

    _firI = new HFir<T>(coefficients, length, 1, 1, 2);
    _firQ = new HFir<T>(coefficients, length, 1, 1, 2);

    _I = new T[blocksize / 2];
    _IOut = new T[blocksize / 2];
    _Q = new T[blocksize / 2];
    _QOut = new T[blocksize / 2];
}

template <class T>
HIqFirFilter<T>::~HIqFirFilter()
{
    HLog("~HIqFirFilter()");
    delete _firI;
    delete _firQ;
    delete _I;
    delete _IOut;
    delete _Q;
    delete _QOut;
}

template <class T>
void HIqFirFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Run FIR filters
    _firI->Filter(src, dest, blocksize);
    _firQ->Filter(&src[1], &dest[1], blocksize);
}

template <class T>
void HIqFirFilter<T>::SetCoefficients(float* coefficients, int length)
{
    _firI->SetCoefficients(coefficients, length);
    _firQ->SetCoefficients(coefficients, length);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HIqFirFilter
template
HIqFirFilter<int8_t>::HIqFirFilter(HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HIqFirFilter<uint8_t>::HIqFirFilter(HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HIqFirFilter<int16_t>::HIqFirFilter(HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HIqFirFilter<int32_t>::HIqFirFilter(HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HIqFirFilter<int8_t>::HIqFirFilter(HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HIqFirFilter<uint8_t>::HIqFirFilter(HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HIqFirFilter<int16_t>::HIqFirFilter(HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HIqFirFilter<int32_t>::HIqFirFilter(HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HIqFirFilter<int8_t>::HIqFirFilter(HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HIqFirFilter<uint8_t>::HIqFirFilter(HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HIqFirFilter<int16_t>::HIqFirFilter(HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HIqFirFilter<int32_t>::HIqFirFilter(HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

// Init
template
void HIqFirFilter<int8_t>::Init(float* coefficients, int length, size_t blocksize);

template
void HIqFirFilter<uint8_t>::Init(float* coefficients, int length, size_t blocksize);

template
void HIqFirFilter<int16_t>::Init(float* coefficients, int length, size_t blocksize);

template
void HIqFirFilter<int32_t>::Init(float* coefficients, int length, size_t blocksize);

// ~HIqFirFilter()
template
HIqFirFilter<int8_t>::~HIqFirFilter();

template
HIqFirFilter<uint8_t>::~HIqFirFilter();

template
HIqFirFilter<int16_t>::~HIqFirFilter();

template
HIqFirFilter<int32_t>::~HIqFirFilter();

// Filter
template
void HIqFirFilter<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HIqFirFilter<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HIqFirFilter<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HIqFirFilter<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// SetCoefficients
template
void HIqFirFilter<int8_t>::SetCoefficients(float* coefficients, int length);

template
void HIqFirFilter<uint8_t>::SetCoefficients(float* coefficients, int length);

template
void HIqFirFilter<int16_t>::SetCoefficients(float* coefficients, int length);

template
void HIqFirFilter<int32_t>::SetCoefficients(float* coefficients, int length);

//! @endcond
#endif
