#ifndef __HMUTE_CPP
#define __HMUTE_CPP

#include <iostream>

#include "hmute.h"

template <class T>
HMute<T>::HMute(HWriter<T>* writer, bool muted, size_t blocksize):
    HFilter<T>(writer, blocksize),
    _muted(muted)
{
    HLog("HMute(HWriter*, %d, %d)", muted, blocksize);
}

template <class T>
HMute<T>::HMute(HWriterConsumer<T>* consumer, bool muted, size_t blocksize):
    HFilter<T>(consumer, blocksize),
    _muted(muted)
{
    HLog("HMute(HWriterConsumer*, %d, %d)", muted, blocksize);
}

template <class T>
HMute<T>::HMute(HReader<T>* reader, bool muted, size_t blocksize):
    HFilter<T>(reader, blocksize),
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
    _muted = muted;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HMute
template
HMute<int8_t>::HMute(HWriter<int8_t>* writer, bool muted, size_t blocksize);

template
HMute<uint8_t>::HMute(HWriter<uint8_t>* writer, bool muted, size_t blocksize);

template
HMute<int16_t>::HMute(HWriter<int16_t>* writer, bool muted, size_t blocksize);

template
HMute<int32_t>::HMute(HWriter<int32_t>* writer, bool muted, size_t blocksize);

template
HMute<int8_t>::HMute(HWriterConsumer<int8_t>* consumer, bool muted, size_t blocksize);

template
HMute<uint8_t>::HMute(HWriterConsumer<uint8_t>* consumer, bool muted, size_t blocksize);

template
HMute<int16_t>::HMute(HWriterConsumer<int16_t>* consumer, bool muted, size_t blocksize);

template
HMute<int32_t>::HMute(HWriterConsumer<int32_t>* consumer, bool muted, size_t blocksize);

template
HMute<int8_t>::HMute(HReader<int8_t>* reader, bool muted, size_t blocksize);

template
HMute<uint8_t>::HMute(HReader<uint8_t>* reader, bool muted, size_t blocksize);

template
HMute<int16_t>::HMute(HReader<int16_t>* reader, bool muted, size_t blocksize);

template
HMute<int32_t>::HMute(HReader<int32_t>* reader, bool muted, size_t blocksize);

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

//! @endcond
#endif