#ifndef __HDELAY_CPP
#define __HDELAY_CPP

#include "hdelay.h"

template <class T>
HDelay<T>::HDelay(HWriter<T>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<T>* probe):
    HFilter<T>(writer, blocksize, probe) {
    HLog("HDelay(HWriter*, blocksize=%d, rate=%d, seconds=%d)", blocksize, rate, seconds);
    Init(blocksize, rate, seconds);
}

template <class T>
HDelay<T>::HDelay(HWriterConsumer<T>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<T>* probe):
    HFilter<T>(consumer, blocksize, probe) {
    HLog("HDelay(HWriter*, blocksize=%d, rate=%d, seconds=%d)", blocksize, rate, seconds);
    Init(blocksize, rate, seconds);
}

template <class T>
HDelay<T>::HDelay(HReader<T>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<T>* probe):
    HFilter<T>(reader, blocksize, probe) {
    HLog("HDelay(HWriter*, blocksize=%d, rate=%d, seconds=%d)", blocksize, rate, seconds);
    Init(blocksize, rate, seconds);
}

template <class T>
HDelay<T>::HDelay(HWriter<T>* writer, size_t blocksize, int blocks, HProbe<T>* probe):
        HFilter<T>(writer, blocksize, probe) {
    HLog("HDelay(HWriter*, blocksize=%d, blocks=%d)", blocksize, blocks);
    Init(blocksize, blocks);
}

template <class T>
HDelay<T>::HDelay(HWriterConsumer<T>* consumer, size_t blocksize, int blocks, HProbe<T>* probe):
        HFilter<T>(consumer, blocksize, probe) {
    HLog("HDelay(HWriter*, blocksize=%d, blocks=%d)", blocksize, blocks);
    Init(blocksize, blocks);
}

template <class T>
HDelay<T>::HDelay(HReader<T>* reader, size_t blocksize, int blocks, HProbe<T>* probe):
        HFilter<T>(reader, blocksize, probe) {
    HLog("HDelay(HWriter*, blocksize=%d, blocks=%d)", blocksize, blocks);
    Init(blocksize, blocks);
}

template <class T>
HDelay<T>::~HDelay()
{
    HLog("~HDelay()");
    delete _buffer;
}

template <class T>
void HDelay<T>::Filter(T* src, T* dest, size_t blocksize)
{
    memcpy((void*) &_buffer[_inPos * blocksize], (void*) src, blocksize * sizeof(T));
    memcpy((void*) dest, (void*) &_buffer[_outPos * blocksize], blocksize * sizeof(T));

    _inPos++;
    if( _inPos >= _buffersize ) {
        _inPos = 0;
    }
    _outPos++;
    if( _outPos >= _buffersize ) {
        _outPos = 0;
    }
 }

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HDelay
template
HDelay<int8_t>::HDelay(HWriter<int8_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int8_t>* probe);

template
HDelay<uint8_t>::HDelay(HWriter<uint8_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<uint8_t>* probe);

template
HDelay<int16_t>::HDelay(HWriter<int16_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int16_t>* probe);

template
HDelay<int32_t>::HDelay(HWriter<int32_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int32_t>* probe);

template
HDelay<int8_t>::HDelay(HWriterConsumer<int8_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int8_t>* probe);

template
HDelay<uint8_t>::HDelay(HWriterConsumer<uint8_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<uint8_t>* probe);

template
HDelay<int16_t>::HDelay(HWriterConsumer<int16_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int16_t>* probe);

template
HDelay<int32_t>::HDelay(HWriterConsumer<int32_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int32_t>* probe);

template
HDelay<int8_t>::HDelay(HReader<int8_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int8_t>* probe);

template
HDelay<uint8_t>::HDelay(HReader<uint8_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<uint8_t>* probe);

template
HDelay<int16_t>::HDelay(HReader<int16_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int16_t>* probe);

template
HDelay<int32_t>::HDelay(HReader<int32_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<int32_t>* probe);

template
HDelay<int8_t>::HDelay(HWriter<int8_t>* writer, size_t blocksize, int blocks, HProbe<int8_t>* probe);

template
HDelay<uint8_t>::HDelay(HWriter<uint8_t>* writer, size_t blocksize, int blocks, HProbe<uint8_t>* probe);

template
HDelay<int16_t>::HDelay(HWriter<int16_t>* writer, size_t blocksize, int blocks, HProbe<int16_t>* probe);

template
HDelay<int32_t>::HDelay(HWriter<int32_t>* writer, size_t blocksize, int blocks, HProbe<int32_t>* probe);

template
HDelay<int8_t>::HDelay(HWriterConsumer<int8_t>* consumer, size_t blocksize, int blocks, HProbe<int8_t>* probe);

template
HDelay<uint8_t>::HDelay(HWriterConsumer<uint8_t>* consumer, size_t blocksize, int blocks, HProbe<uint8_t>* probe);

template
HDelay<int16_t>::HDelay(HWriterConsumer<int16_t>* consumer, size_t blocksize, int blocks, HProbe<int16_t>* probe);

template
HDelay<int32_t>::HDelay(HWriterConsumer<int32_t>* consumer, size_t blocksize, int blocks, HProbe<int32_t>* probe);

template
HDelay<int8_t>::HDelay(HReader<int8_t>* reader, size_t blocksize, int blocks, HProbe<int8_t>* probe);

template
HDelay<uint8_t>::HDelay(HReader<uint8_t>* reader, size_t blocksize, int blocks, HProbe<uint8_t>* probe);

template
HDelay<int16_t>::HDelay(HReader<int16_t>* reader, size_t blocksize, int blocks, HProbe<int16_t>* probe);

template
HDelay<int32_t>::HDelay(HReader<int32_t>* reader, size_t blocksize, int blocks, HProbe<int32_t>* probe);

// ~HDelay()
template
HDelay<int8_t>::~HDelay();

template
HDelay<uint8_t>::~HDelay();

template
HDelay<int16_t>::~HDelay();

template
HDelay<int32_t>::~HDelay();

// Filter
template
void HDelay<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HDelay<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HDelay<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HDelay<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif