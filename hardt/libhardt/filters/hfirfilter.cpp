#ifndef __HFIRFILTER_CPP
#define __HFIRFILTER_CPP

#include "hfirfilter.h"

template <class T>
HFirFilter<T>::HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(writer, blocksize, probe) {

    HLog("HFirFilter(HWriter*)");
    HLog("_length = %d", length);
    Init(coefficients, length);
}

template <class T>
HFirFilter<T>::HFirFilter(HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(consumer, blocksize, probe) {

    HLog("HFirFilter(HWriterConsumer*)");
    HLog("_length = %d", length);
    Init(coefficients, length);

    consumer->SetWriter(this);
}

template <class T>
HFirFilter<T>::HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(reader, blocksize, probe) {

    HLog("HFirFilter(HReader*)");
    HLog("_length = %d", length);
    Init(coefficients, length);
}

template <class T>
void HFirFilter<T>::Init(float* coefficients, int length)
{
    HLog("Init(float*, %d)", length);
    _fir = new HFir<T>(coefficients, length);
}

template <class T>
HFirFilter<T>::~HFirFilter()
{
    HLog("~HFirFilter()");
    delete _fir;
}

template <class T>
void HFirFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Run FIR filter
    _fir->Filter(src, dest, blocksize);
}

template <class T>
void HFirFilter<T>::SetCoefficients(float* coefficients, int length)
{
    _fir->SetCoefficients(coefficients, length);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HFirFilter
template
HFirFilter<int8_t>::HFirFilter(HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HFirFilter<uint8_t>::HFirFilter(HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HFirFilter<int16_t>::HFirFilter(HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HFirFilter<int32_t>::HFirFilter(HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HFirFilter<int8_t>::HFirFilter(HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HFirFilter<uint8_t>::HFirFilter(HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HFirFilter<int16_t>::HFirFilter(HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HFirFilter<int32_t>::HFirFilter(HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HFirFilter<int8_t>::HFirFilter(HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HFirFilter<uint8_t>::HFirFilter(HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HFirFilter<int16_t>::HFirFilter(HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HFirFilter<int32_t>::HFirFilter(HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

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

//! @endcond
#endif
