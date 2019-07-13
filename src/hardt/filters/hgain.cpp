#ifndef __HGAIN_CPP
#define __HGAIN_CPP

#include "hgain.h"

template <class T>
HGain<T>::HGain(HWriter<T>* writer, float gain, size_t blocksize):
    HFilter<T>(writer, blocksize),
    _gain(gain)
{
    HLog("HGain(HWriter*, %f, %d)", gain, blocksize);
}

template <class T>
HGain<T>::HGain(HReader<T>* reader, float gain, size_t blocksize):
    HFilter<T>(reader, blocksize),
    _gain(gain)
{
    HLog("HGain(HReader*, %f, %d)", gain, blocksize);
}

template <class T>
HGain<T>::~HGain()
{
    HLog("~HGain()");
}

template <class T>
void HGain<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Run gain filter
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = src[i] * _gain;
    }
}

template <class T>
void HGain<T>::SetGain(float gain)
{
    _gain = gain;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HGain
template
HGain<int8_t>::HGain(HWriter<int8_t>* writer, float gain, size_t blocksize);

template
HGain<uint8_t>::HGain(HWriter<uint8_t>* writer, float gain, size_t blocksize);

template
HGain<int16_t>::HGain(HWriter<int16_t>* writer, float gain, size_t blocksize);

template
HGain<int32_t>::HGain(HWriter<int32_t>* writer, float gain, size_t blocksize);

template
HGain<int8_t>::HGain(HReader<int8_t>* reader, float gain, size_t blocksize);

template
HGain<uint8_t>::HGain(HReader<uint8_t>* reader, float gain, size_t blocksize);

template
HGain<int16_t>::HGain(HReader<int16_t>* reader, float gain, size_t blocksize);

template
HGain<int32_t>::HGain(HReader<int32_t>* reader, float gain, size_t blocksize);

// ~HGain()
template
HGain<int8_t>::~HGain();

template
HGain<uint8_t>::~HGain();

template
HGain<int16_t>::~HGain();

template
HGain<int32_t>::~HGain();

// Filter
template
void HGain<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HGain<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HGain<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HGain<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// SetGain()
template
void HGain<int8_t>::SetGain(float gain);

template
void HGain<uint8_t>::SetGain(float gain);

template
void HGain<int16_t>::SetGain(float gain);

template
void HGain<int32_t>::SetGain(float gain);

#endif