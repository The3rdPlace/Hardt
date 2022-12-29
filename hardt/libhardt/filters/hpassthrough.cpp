#ifndef __HPASSTHROUGH_CPP
#define __HPASSTHROUGH_CPP

#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hprobe.h"

#include "hpassthrough.h"

template <class T>
HPassThrough<T>::HPassThrough(std::string id, HWriter<T>* writer, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, writer, blocksize, probe)
{
    HLog("HPassThrough(HWriter*, %d)", blocksize);
}

template <class T>
HPassThrough<T>::HPassThrough(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, consumer, blocksize, probe)
{
    HLog("HPassThrough(HWriterConsumer*, %d)", blocksize);
}

template <class T>
HPassThrough<T>::HPassThrough(std::string id, HReader<T>* reader, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, reader, blocksize, probe)
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
HPassThrough<int8_t>::HPassThrough(std::string id, HWriter<int8_t>* writer, size_t blocksize, HProbe<int8_t>* probe);

template
HPassThrough<uint8_t>::HPassThrough(std::string id, HWriter<uint8_t>* writer, size_t blocksize, HProbe<uint8_t>* probe);

template
HPassThrough<int16_t>::HPassThrough(std::string id, HWriter<int16_t>* writer, size_t blocksize, HProbe<int16_t>* probe);

template
HPassThrough<int32_t>::HPassThrough(std::string id, HWriter<int32_t>* writer, size_t blocksize, HProbe<int32_t>* probe);

template
HPassThrough<int8_t>::HPassThrough(std::string id, HWriterConsumer<int8_t>* consumer, size_t blocksize, HProbe<int8_t>* probe);

template
HPassThrough<uint8_t>::HPassThrough(std::string id, HWriterConsumer<uint8_t>* consumer, size_t blocksize, HProbe<uint8_t>* probe);

template
HPassThrough<int16_t>::HPassThrough(std::string id, HWriterConsumer<int16_t>* consumer, size_t blocksize, HProbe<int16_t>* probe);

template
HPassThrough<int32_t>::HPassThrough(std::string id, HWriterConsumer<int32_t>* consumer, size_t blocksize, HProbe<int32_t>* probe);

template
HPassThrough<int8_t>::HPassThrough(std::string id, HReader<int8_t>* reader, size_t blocksize, HProbe<int8_t>* probe);

template
HPassThrough<uint8_t>::HPassThrough(std::string id, HReader<uint8_t>* reader, size_t blocksize, HProbe<uint8_t>* probe);

template
HPassThrough<int16_t>::HPassThrough(std::string id, HReader<int16_t>* reader, size_t blocksize, HProbe<int16_t>* probe);

template
HPassThrough<int32_t>::HPassThrough(std::string id, HReader<int32_t>* reader, size_t blocksize, HProbe<int32_t>* probe);

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