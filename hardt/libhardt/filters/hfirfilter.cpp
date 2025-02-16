#ifndef __HFIRFILTER_CPP
#define __HFIRFILTER_CPP

#include "hfirfilter.h"

template <class T>
HFirFilter<T>::HFirFilter(std::string id, HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, writer, blocksize) {

    HLog("HFirFilter(HWriter*)");
    HLog("_length = %d", length);
    Init(coefficients, length);
}

template <class T>
HFirFilter<T>::HFirFilter(std::string id, HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, consumer, blocksize) {

    HLog("HFirFilter(HWriterConsumer*)");
    HLog("_length = %d", length);
    Init(coefficients, length);

    consumer->SetWriter(this);
}

template <class T>
HFirFilter<T>::HFirFilter(std::string id, HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, reader, blocksize) {

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
HFirFilter<int8_t>::HFirFilter(std::string id, HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(std::string id, HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(std::string id, HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(std::string id, HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int8_t>::HFirFilter(std::string id, HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(std::string id, HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(std::string id, HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(std::string id, HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int8_t>::HFirFilter(std::string id, HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<uint8_t>::HFirFilter(std::string id, HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int16_t>::HFirFilter(std::string id, HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HFirFilter<int32_t>::HFirFilter(std::string id, HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

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
