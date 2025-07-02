#ifndef __HBREAKER_CPP
#define __HBREAKER_CPP

#include <iostream>

#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hbreaker.h"

template <class T>
HBreaker<T>::HBreaker(std::string id, HWriter<T>* writer, bool off, size_t blocksize):
    HFilter<T>(id, writer, blocksize),
    _off(off)
{
    HLog("HBreaker(HWriter*, %d, %d)", off, blocksize);
}

template <class T>
HBreaker<T>::HBreaker(std::string id, HWriterConsumer<T>* consumer, bool off, size_t blocksize):
    HFilter<T>(id, consumer, blocksize),
    _off(off)
{
    HLog("HBreaker(HWriterConsumer*, %d, %d)", off, blocksize);
}

template <class T>
HBreaker<T>::HBreaker(std::string id, HReader<T>* reader, bool off, size_t blocksize):
    HFilter<T>(id, reader, blocksize),
    _off(off)
{
    HLog("HBreaker(HReader*, %d, %d)", off, blocksize);
}

template <class T>
HBreaker<T>::~HBreaker()
{
    HLog("~HBreaker()");
}

template <class T>
void HBreaker<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Just pass the samples through
    memcpy((void*) dest, (void*) src, blocksize * sizeof(T));
}

template <class T>
void HBreaker<T>::SetOff(bool off)
{
    if( off && !_off )
    {
        HLog("Sending Stop() to reader/writer");
        _off = off;
        HFilter<T>::Stop();
    }
    else if( !off && _off )
    {
        HLog("Sending Start() to reader/writer");
        _off = off;
        HFilter<T>::Start();
    }
    else
    {
        HLog("Huhhh?");
    }
    
}

template <class T>
bool HBreaker<T>::Start()
{
    if( !_off )
    {
        HLog("Propagating Start() to reader/writer");
        return HFilter<T>::Start();
    }

    HLog("Discarding Start() to reader/writer");
    return true;
}

template <class T>
bool HBreaker<T>::Stop()
{
    if( !_off )
    {
        HLog("Propagating Stop() to reader/writer");
        HFilter<T>::Stop();
    }

    HLog("Discarding Stop() to reader/writer");
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HBreaker
template
HBreaker<int8_t>::HBreaker(std::string id, HWriter<int8_t>* writer, bool muted, size_t blocksize);

template
HBreaker<uint8_t>::HBreaker(std::string id, HWriter<uint8_t>* writer, bool muted, size_t blocksize);

template
HBreaker<int16_t>::HBreaker(std::string id, HWriter<int16_t>* writer, bool muted, size_t blocksize);

template
HBreaker<int32_t>::HBreaker(std::string id, HWriter<int32_t>* writer, bool muted, size_t blocksize);

template
HBreaker<int8_t>::HBreaker(std::string id, HWriterConsumer<int8_t>* consumer, bool muted, size_t blocksize);

template
HBreaker<uint8_t>::HBreaker(std::string id, HWriterConsumer<uint8_t>* consumer, bool muted, size_t blocksize);

template
HBreaker<int16_t>::HBreaker(std::string id, HWriterConsumer<int16_t>* consumer, bool muted, size_t blocksize);

template
HBreaker<int32_t>::HBreaker(std::string id, HWriterConsumer<int32_t>* consumer, bool muted, size_t blocksize);

template
HBreaker<int8_t>::HBreaker(std::string id, HReader<int8_t>* reader, bool muted, size_t blocksize);

template
HBreaker<uint8_t>::HBreaker(std::string id, HReader<uint8_t>* reader, bool muted, size_t blocksize);

template
HBreaker<int16_t>::HBreaker(std::string id, HReader<int16_t>* reader, bool muted, size_t blocksize);

template
HBreaker<int32_t>::HBreaker(std::string id, HReader<int32_t>* reader, bool muted, size_t blocksize);

// ~HBreaker()
template
HBreaker<int8_t>::~HBreaker();

template
HBreaker<uint8_t>::~HBreaker();

template
HBreaker<int16_t>::~HBreaker();

template
HBreaker<int32_t>::~HBreaker();

// Filter
template
void HBreaker<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HBreaker<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HBreaker<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HBreaker<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// SetMuted()
template
void HBreaker<int8_t>::SetOff(bool off);

template
void HBreaker<uint8_t>::SetOff(bool off);

template
void HBreaker<int16_t>::SetOff(bool off);

template
void HBreaker<int32_t>::SetOff(bool off);

// Start()
template
bool HBreaker<int8_t>::Start();

template
bool HBreaker<uint8_t>::Start();

template
bool HBreaker<int16_t>::Start();

template
bool HBreaker<int32_t>::Start();

// Stop()
template
bool HBreaker<int8_t>::Stop();

template
bool HBreaker<uint8_t>::Stop();

template
bool HBreaker<int16_t>::Stop();

template
bool HBreaker<int32_t>::Stop();

//! @endcond
#endif