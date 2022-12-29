#ifndef __HCLICKREMOVINGFILTER_CPP
#define __HCLICKREMOVINGFILTER_CPP

#include "hclickremovingfilter.h"
#include "hardt.h"

template <class T>
HClickRemovingFilter<T>::HClickRemovingFilter(std::string id, HWriter<T>* writer, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, writer, blocksize, probe)
{
    HLog("HClickRemovingFilter(HWriter*, %d)", blocksize);
}

template <class T>
HClickRemovingFilter<T>::HClickRemovingFilter(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, consumer, blocksize, probe)
{
    HLog("HClickRemovingFilter(HWriterConsumer*, %d)", blocksize);
}

template <class T>
HClickRemovingFilter<T>::HClickRemovingFilter(std::string id, HReader<T>* reader, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, reader, blocksize, probe)
{
    HLog("HClickRemovingFilter(HReader*, %d)", blocksize);
}

template <class T>
HClickRemovingFilter<T>::~HClickRemovingFilter()
{
    HLog("~HClickRemovingFilter()");
}

template <class T>
void HClickRemovingFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    long double sum = 0;

    // Run filter
    for( int i = 0; i < blocksize; i++ )
    {
        sum += src[i];
    }
    sum *= 1.5;
    T average = (T) sum / blocksize;

    // Run filter
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = src[i] > average ? 0 : src[i];
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HClickRemovingFilter
template
HClickRemovingFilter<int8_t>::HClickRemovingFilter(std::string id, HWriter<int8_t>* writer, size_t blocksize, HProbe<int8_t>* probe);

template
HClickRemovingFilter<uint8_t>::HClickRemovingFilter(std::string id, HWriter<uint8_t>* writer, size_t blocksize, HProbe<uint8_t>* probe);

template
HClickRemovingFilter<int16_t>::HClickRemovingFilter(std::string id, HWriter<int16_t>* writer, size_t blocksize, HProbe<int16_t>* probe);

template
HClickRemovingFilter<int32_t>::HClickRemovingFilter(std::string id, HWriter<int32_t>* writer, size_t blocksize, HProbe<int32_t>* probe);

template
HClickRemovingFilter<int8_t>::HClickRemovingFilter(std::string id, HWriterConsumer<int8_t>* consumer, size_t blocksize, HProbe<int8_t>* probe);

template
HClickRemovingFilter<uint8_t>::HClickRemovingFilter(std::string id, HWriterConsumer<uint8_t>* consumer, size_t blocksize, HProbe<uint8_t>* probe);

template
HClickRemovingFilter<int16_t>::HClickRemovingFilter(std::string id, HWriterConsumer<int16_t>* consumer, size_t blocksize, HProbe<int16_t>* probe);

template
HClickRemovingFilter<int32_t>::HClickRemovingFilter(std::string id, HWriterConsumer<int32_t>* consumer, size_t blocksize, HProbe<int32_t>* probe);

template
HClickRemovingFilter<int8_t>::HClickRemovingFilter(std::string id, HReader<int8_t>* reader, size_t blocksize, HProbe<int8_t>* probe);

template
HClickRemovingFilter<uint8_t>::HClickRemovingFilter(std::string id, HReader<uint8_t>* reader, size_t blocksize, HProbe<uint8_t>* probe);

template
HClickRemovingFilter<int16_t>::HClickRemovingFilter(std::string id, HReader<int16_t>* reader, size_t blocksize, HProbe<int16_t>* probe);

template
HClickRemovingFilter<int32_t>::HClickRemovingFilter(std::string id, HReader<int32_t>* reader, size_t blocksize, HProbe<int32_t>* probe);

// ~HClickRemovingFilter()
template
HClickRemovingFilter<int8_t>::~HClickRemovingFilter();

template
HClickRemovingFilter<uint8_t>::~HClickRemovingFilter();

template
HClickRemovingFilter<int16_t>::~HClickRemovingFilter();

template
HClickRemovingFilter<int32_t>::~HClickRemovingFilter();

// Filter
template
void HClickRemovingFilter<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HClickRemovingFilter<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HClickRemovingFilter<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HClickRemovingFilter<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif