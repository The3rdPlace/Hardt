#ifndef __HGAIN_CPP
#define __HGAIN_CPP

#include "hgain.h"

template <class T>
HGain<T>::HGain(std::string id, HWriter<T>* writer, float gain, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, writer, blocksize, probe),
    _gain(gain)
{
    HLog("HGain(HWriter*, %f, %d)", gain, blocksize);
}

template <class T>
HGain<T>::HGain(std::string id, HWriterConsumer<T>* consumer, float gain, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, consumer, blocksize, probe),
    _gain(gain)
{
    HLog("HGain(HWriterConsumer*, %f, %d)", gain, blocksize);
}

template <class T>
HGain<T>::HGain(std::string id, HReader<T>* reader, float gain, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, reader, blocksize, probe),
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
    _gain = gain >= 0 ? gain : 0;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HGain
template
HGain<int8_t>::HGain(std::string id, HWriter<int8_t>* writer, float gain, size_t blocksize, HProbe<int8_t>* probe);

template
HGain<uint8_t>::HGain(std::string id, HWriter<uint8_t>* writer, float gain, size_t blocksize, HProbe<uint8_t>* probe);

template
HGain<int16_t>::HGain(std::string id, HWriter<int16_t>* writer, float gain, size_t blocksize, HProbe<int16_t>* probe);

template
HGain<int32_t>::HGain(std::string id, HWriter<int32_t>* writer, float gain, size_t blocksize, HProbe<int32_t>* probe);

template
HGain<int8_t>::HGain(std::string id, HWriterConsumer<int8_t>* consumer, float gain, size_t blocksize, HProbe<int8_t>* probe);

template
HGain<uint8_t>::HGain(std::string id, HWriterConsumer<uint8_t>* consumer, float gain, size_t blocksize, HProbe<uint8_t>* probe);

template
HGain<int16_t>::HGain(std::string id, HWriterConsumer<int16_t>* consumer, float gain, size_t blocksize, HProbe<int16_t>* probe);

template
HGain<int32_t>::HGain(std::string id, HWriterConsumer<int32_t>* consumer, float gain, size_t blocksize, HProbe<int32_t>* probe);

template
HGain<int8_t>::HGain(std::string id, HReader<int8_t>* reader, float gain, size_t blocksize, HProbe<int8_t>* probe);

template
HGain<uint8_t>::HGain(std::string id, HReader<uint8_t>* reader, float gain, size_t blocksize, HProbe<uint8_t>* probe);

template
HGain<int16_t>::HGain(std::string id, HReader<int16_t>* reader, float gain, size_t blocksize, HProbe<int16_t>* probe);

template
HGain<int32_t>::HGain(std::string id, HReader<int32_t>* reader, float gain, size_t blocksize, HProbe<int32_t>* probe);

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

//! @endcond
#endif