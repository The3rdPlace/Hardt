#ifndef __HIQFIRFILTER_CPP
#define __HIQFIRFILTER_CPP

#include "hiqfirfilter.h"

template <class T>
HIqFirFilter<T>::HIqFirFilter(std::string id, HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, writer, blocksize) {

    HLog("HIqFirFilter(HWriter*)");
    HLog("_length = %d", length);
    Init(coefficients, length, blocksize);
}

template <class T>
HIqFirFilter<T>::HIqFirFilter(std::string id, HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, consumer, blocksize) {

    HLog("HIqFirFilter(HWriterConsumer*)");
    HLog("_length = %d", length);
    Init(coefficients, length, blocksize);

    consumer->SetWriter(this);
}

template <class T>
HIqFirFilter<T>::HIqFirFilter(std::string id, HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    HFilter<T>(id, reader, blocksize) {

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
}

template <class T>
HIqFirFilter<T>::~HIqFirFilter()
{
    HLog("~HIqFirFilter()");
    delete _firI;
    delete _firQ;
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
HIqFirFilter<int8_t>::HIqFirFilter(std::string id, HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<uint8_t>::HIqFirFilter(std::string id, HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int16_t>::HIqFirFilter(std::string id, HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int32_t>::HIqFirFilter(std::string id, HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int8_t>::HIqFirFilter(std::string id, HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<uint8_t>::HIqFirFilter(std::string id, HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int16_t>::HIqFirFilter(std::string id, HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int32_t>::HIqFirFilter(std::string id, HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int8_t>::HIqFirFilter(std::string id, HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<uint8_t>::HIqFirFilter(std::string id, HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int16_t>::HIqFirFilter(std::string id, HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HIqFirFilter<int32_t>::HIqFirFilter(std::string id, HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

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
