#ifndef __HAGC_CPP
#define __HAGC_CPP

#include "hagc.h"

template <class T>
HAgc<T>::HAgc(HWriter<T>* writer, T upper, T limit, float rate, size_t blocksize, HProbe<T>* probe):
    HGain<T>(writer, 1, blocksize, probe),
    _upper(upper),
    _limit(limit),
    _rate(rate)
{
    HLog("HAgc(HWriter*, %d, %d, %f, %d)", upper, limit, rate, blocksize);
}

template <class T>
HAgc<T>::HAgc(HWriterConsumer<T>* consumer, T upper, T limit, float rate, size_t blocksize, HProbe<T>* probe):
    HGain<T>(consumer, 1, blocksize, probe),
    _upper(upper),
    _limit(limit),
    _rate(rate)
{
    HLog("HAgc(HWriterConsumer*, %d, %d, %f, %d)", upper, limit, rate, blocksize);
}

template <class T>
HAgc<T>::HAgc(HReader<T>* reader, T upper, T limit, float rate, size_t blocksize, HProbe<T>* probe):
    HGain<T>(reader, 1, blocksize, probe),
    _upper(upper),
    _limit(limit),
    _rate(rate)
{
    HLog("HAgc(HReader*, %d, %d, %f, %d)", upper, limit, rate, blocksize);
}

template <class T>
HAgc<T>::~HAgc()
{
    HLog("~HAgc()");
}

template <class T>
void HAgc<T>::Filter(T* src, T* dest, size_t blocksize)
{
    HGain<T>::Filter(src, dest, blocksize);

    T max = *std::max_element(dest, dest + blocksize);
    if( max < _upper && this->GetGain() < _limit )
    {
        this->SetGain(this->GetGain() + _rate);
    }
    if( max > _upper && this->GetGain() > 0)
    {
        this->SetGain(this->GetGain() - _rate);
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HAgc
template
HAgc<int8_t>::HAgc(HWriter<int8_t>* writer, int8_t upper, int8_t limit, float rate, size_t blocksize, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(HWriter<uint8_t>* writer, uint8_t upper, uint8_t limit, float rate, size_t blocksize, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(HWriter<int16_t>* writer, int16_t upper, int16_t limit, float rate, size_t blocksize, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(HWriter<int32_t>* writer, int32_t upper, int32_t limit, float rate, size_t blocksize, HProbe<int32_t>* probe);

template
HAgc<int8_t>::HAgc(HWriterConsumer<int8_t>* consumer, int8_t upper, int8_t limit, float rate, size_t blocksize, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(HWriterConsumer<uint8_t>* consumer, uint8_t upper, uint8_t limit, float rate, size_t blocksize, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(HWriterConsumer<int16_t>* consumer, int16_t upper, int16_t limit, float rate, size_t blocksize, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(HWriterConsumer<int32_t>* consumer, int32_t upper, int32_t limit, float rate, size_t blocksize, HProbe<int32_t>* probe);

template
HAgc<int8_t>::HAgc(HReader<int8_t>* reader, int8_t upper, int8_t limit, float rate, size_t blocksize, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(HReader<uint8_t>* reader, uint8_t upper, uint8_t limit, float rate, size_t blocksize, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(HReader<int16_t>* reader, int16_t upper, int16_t limit, float rate, size_t blocksize, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(HReader<int32_t>* reader, int32_t upper, int32_t limit, float rate, size_t blocksize, HProbe<int32_t>* probe);

// ~HAgc()
template
HAgc<int8_t>::~HAgc();

template
HAgc<uint8_t>::~HAgc();

template
HAgc<int16_t>::~HAgc();

template
HAgc<int32_t>::~HAgc();

// Filter
template
void HAgc<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HAgc<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HAgc<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HAgc<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif