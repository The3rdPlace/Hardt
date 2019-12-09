#ifndef __HPASSTHROUGH_CPP
#define __HPASSTHROUGH_CPP

#include "hpassthrough.h"

template <class T>
HPassThrough<T>::HPassThrough(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(writer, blocksize, probe)
{
    HLog("HPassThrough(HWriter*, %d)", blocksize);
}

template <class T>
HPassThrough<T>::HPassThrough(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(consumer, blocksize, probe)
{
    HLog("HPassThrough(HWriterConsumer*, %d)", blocksize);
}

template <class T>
HPassThrough<T>::HPassThrough(HReader<T>* reader, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(reader, blocksize, probe)
{
    HLog("HPassThrough(HReader*, %d)", blocksize);
}

template <class T>
HPassThrough<T>::~HPassThrough()
{
    HLog("~HPassThrough()");
}

template <class T>
void HPassThrough<T>::Filter(T* src, T* dest, size_t blocksize)
{
    memcpy((void*) dest, (void*) src, blocksize * sizeof(T));
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HPassThrough
template
HPassThrough<int8_t>::HPassThrough(HWriter<int8_t>* writer, size_t blocksize, HProbe<int8_t>* probe);

template
HPassThrough<uint8_t>::HPassThrough(HWriter<uint8_t>* writer, size_t blocksize, HProbe<uint8_t>* probe);

template
HPassThrough<int16_t>::HPassThrough(HWriter<int16_t>* writer, size_t blocksize, HProbe<int16_t>* probe);

template
HPassThrough<int32_t>::HPassThrough(HWriter<int32_t>* writer, size_t blocksize, HProbe<int32_t>* probe);

template
HPassThrough<int8_t>::HPassThrough(HWriterConsumer<int8_t>* consumer, size_t blocksize, HProbe<int8_t>* probe);

template
HPassThrough<uint8_t>::HPassThrough(HWriterConsumer<uint8_t>* consumer, size_t blocksize, HProbe<uint8_t>* probe);

template
HPassThrough<int16_t>::HPassThrough(HWriterConsumer<int16_t>* consumer, size_t blocksize, HProbe<int16_t>* probe);

template
HPassThrough<int32_t>::HPassThrough(HWriterConsumer<int32_t>* consumer, size_t blocksize, HProbe<int32_t>* probe);

template
HPassThrough<int8_t>::HPassThrough(HReader<int8_t>* reader, size_t blocksize, HProbe<int8_t>* probe);

template
HPassThrough<uint8_t>::HPassThrough(HReader<uint8_t>* reader, size_t blocksize, HProbe<uint8_t>* probe);

template
HPassThrough<int16_t>::HPassThrough(HReader<int16_t>* reader, size_t blocksize, HProbe<int16_t>* probe);

template
HPassThrough<int32_t>::HPassThrough(HReader<int32_t>* reader, size_t blocksize, HProbe<int32_t>* probe);

// ~HPassThrough()
template
HPassThrough<int8_t>::~HPassThrough();

template
HPassThrough<uint8_t>::~HPassThrough();

template
HPassThrough<int16_t>::~HPassThrough();

template
HPassThrough<int32_t>::~HPassThrough();

// Filter
template
void HPassThrough<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HPassThrough<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HPassThrough<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HPassThrough<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif