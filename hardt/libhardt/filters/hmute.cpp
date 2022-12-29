#ifndef __HMUTE_CPP
#define __HMUTE_CPP

#include <iostream>

#include "hmute.h"

template <class T>
HMute<T>::HMute(std::string id, HWriter<T>* writer, bool muted, size_t blocksize):
    HFilter<T>(id, writer, blocksize),
    _muted(muted)
{
    HLog("HMute(HWriter*, %d, %d)", muted, blocksize);
}

template <class T>
HMute<T>::HMute(std::string id, HWriterConsumer<T>* consumer, bool muted, size_t blocksize):
    HFilter<T>(id, consumer, blocksize),
    _muted(muted)
{
    HLog("HMute(HWriterConsumer*, %d, %d)", muted, blocksize);
}

template <class T>
HMute<T>::HMute(std::string id, HReader<T>* reader, bool muted, size_t blocksize):
    HFilter<T>(id, reader, blocksize),
    _muted(muted)
{
    HLog("HMute(HReader*, %d, %d)", muted, blocksize);
}

template <class T>
HMute<T>::~HMute()
{
    HLog("~HMute()");
}

template <class T>
void HMute<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // If the filter is muted, zero all samples
    if( _muted )
    {
        T zero = 0;
        for( int i = 0; i < blocksize; i++ )
        {
            dest[i] = zero;
        }
        return;
    }

    // Otherwise, just pass the samples through
    memcpy((void*) dest, (void*) src, blocksize * sizeof(T));
}

template <class T>
void HMute<T>::SetMuted(bool muted)
{
    if( muted && !_muted )
    {
        HLog("Sending Stop() to reader/writer");
        _muted = muted;
        HFilter<T>::Stop();
    }
    else if( !muted && _muted )
    {
        HLog("Sending Start() to reader/writer");
        _muted = muted;
        HFilter<T>::Start();
    }
    else
    {
        HLog("Huhhh?");
    }
}

template <class T>
bool HMute<T>::Start()
{
    if( !_muted )
    {
        HLog("Propagating Start() to reader/writer");
        return HFilter<T>::Start();
    }

    HLog("Discarding Start() to reader/writer");
    return true;
}

template <class T>
bool HMute<T>::Stop()
{
    if( !_muted )
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

// HMute
template
HMute<int8_t>::HMute(std::string id, HWriter<int8_t>* writer, bool muted, size_t blocksize);

template
HMute<uint8_t>::HMute(std::string id, HWriter<uint8_t>* writer, bool muted, size_t blocksize);

template
HMute<int16_t>::HMute(std::string id, HWriter<int16_t>* writer, bool muted, size_t blocksize);

template
HMute<int32_t>::HMute(std::string id, HWriter<int32_t>* writer, bool muted, size_t blocksize);

template
HMute<int8_t>::HMute(std::string id, HWriterConsumer<int8_t>* consumer, bool muted, size_t blocksize);

template
HMute<uint8_t>::HMute(std::string id, HWriterConsumer<uint8_t>* consumer, bool muted, size_t blocksize);

template
HMute<int16_t>::HMute(std::string id, HWriterConsumer<int16_t>* consumer, bool muted, size_t blocksize);

template
HMute<int32_t>::HMute(std::string id, HWriterConsumer<int32_t>* consumer, bool muted, size_t blocksize);

template
HMute<int8_t>::HMute(std::string id, HReader<int8_t>* reader, bool muted, size_t blocksize);

template
HMute<uint8_t>::HMute(std::string id, HReader<uint8_t>* reader, bool muted, size_t blocksize);

template
HMute<int16_t>::HMute(std::string id, HReader<int16_t>* reader, bool muted, size_t blocksize);

template
HMute<int32_t>::HMute(std::string id, HReader<int32_t>* reader, bool muted, size_t blocksize);

// ~HMute()
template
HMute<int8_t>::~HMute();

template
HMute<uint8_t>::~HMute();

template
HMute<int16_t>::~HMute();

template
HMute<int32_t>::~HMute();

// Filter
template
void HMute<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HMute<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HMute<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HMute<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// SetMuted()
template
void HMute<int8_t>::SetMuted(bool muted);

template
void HMute<uint8_t>::SetMuted(bool muted);

template
void HMute<int16_t>::SetMuted(bool muted);

template
void HMute<int32_t>::SetMuted(bool muted);

// Start()
template
bool HMute<int8_t>::Start();

template
bool HMute<uint8_t>::Start();

template
bool HMute<int16_t>::Start();

template
bool HMute<int32_t>::Start();

// Stop()
template
bool HMute<int8_t>::Stop();

template
bool HMute<uint8_t>::Stop();

template
bool HMute<int16_t>::Stop();

template
bool HMute<int32_t>::Stop();

//! @endcond
#endif