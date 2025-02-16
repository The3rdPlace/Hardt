#ifndef __HDELAY_CPP
#define __HDELAY_CPP

#include "hdelay.h"

template <class T>
HDelay<T>::HDelay(std::string id, HWriter<T>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds):
    HFilter<T>(id, writer, blocksize) {
    HLog("HDelay(HWriter*, blocksize=%d, rate=%d, seconds=%d)", blocksize, rate, seconds);
    Init(blocksize, rate, seconds);
}

template <class T>
HDelay<T>::HDelay(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds):
    HFilter<T>(id, consumer, blocksize) {
    HLog("HDelay(HWriter*, blocksize=%d, rate=%d, seconds=%d)", blocksize, rate, seconds);
    Init(blocksize, rate, seconds);
}

template <class T>
HDelay<T>::HDelay(std::string id, HReader<T>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds):
    HFilter<T>(id, reader, blocksize) {
    HLog("HDelay(HWriter*, blocksize=%d, rate=%d, seconds=%d)", blocksize, rate, seconds);
    Init(blocksize, rate, seconds);
}

template <class T>
HDelay<T>::HDelay(std::string id, HWriter<T>* writer, size_t blocksize, int blocks):
        HFilter<T>(id, writer, blocksize) {
    HLog("HDelay(HWriter*, blocksize=%d, blocks=%d)", blocksize, blocks);
    Init(blocksize, blocks);
}

template <class T>
HDelay<T>::HDelay(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, int blocks):
        HFilter<T>(id, consumer, blocksize) {
    HLog("HDelay(HWriter*, blocksize=%d, blocks=%d)", blocksize, blocks);
    Init(blocksize, blocks);
}

template <class T>
HDelay<T>::HDelay(std::string id, HReader<T>* reader, size_t blocksize, int blocks):
        HFilter<T>(id, reader, blocksize) {
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
HDelay<int8_t>::HDelay(std::string id, HWriter<int8_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<uint8_t>::HDelay(std::string id, HWriter<uint8_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int16_t>::HDelay(std::string id, HWriter<int16_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int32_t>::HDelay(std::string id, HWriter<int32_t>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int8_t>::HDelay(std::string id, HWriterConsumer<int8_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<uint8_t>::HDelay(std::string id, HWriterConsumer<uint8_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int16_t>::HDelay(std::string id, HWriterConsumer<int16_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int32_t>::HDelay(std::string id, HWriterConsumer<int32_t>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int8_t>::HDelay(std::string id, HReader<int8_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<uint8_t>::HDelay(std::string id, HReader<uint8_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int16_t>::HDelay(std::string id, HReader<int16_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int32_t>::HDelay(std::string id, HReader<int32_t>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

template
HDelay<int8_t>::HDelay(std::string id, HWriter<int8_t>* writer, size_t blocksize, int blocks);

template
HDelay<uint8_t>::HDelay(std::string id, HWriter<uint8_t>* writer, size_t blocksize, int blocks);

template
HDelay<int16_t>::HDelay(std::string id, HWriter<int16_t>* writer, size_t blocksize, int blocks);

template
HDelay<int32_t>::HDelay(std::string id, HWriter<int32_t>* writer, size_t blocksize, int blocks);

template
HDelay<int8_t>::HDelay(std::string id, HWriterConsumer<int8_t>* consumer, size_t blocksize, int blocks);

template
HDelay<uint8_t>::HDelay(std::string id, HWriterConsumer<uint8_t>* consumer, size_t blocksize, int blocks);

template
HDelay<int16_t>::HDelay(std::string id, HWriterConsumer<int16_t>* consumer, size_t blocksize, int blocks);

template
HDelay<int32_t>::HDelay(std::string id, HWriterConsumer<int32_t>* consumer, size_t blocksize, int blocks);

template
HDelay<int8_t>::HDelay(std::string id, HReader<int8_t>* reader, size_t blocksize, int blocks);

template
HDelay<uint8_t>::HDelay(std::string id, HReader<uint8_t>* reader, size_t blocksize, int blocks);

template
HDelay<int16_t>::HDelay(std::string id, HReader<int16_t>* reader, size_t blocksize, int blocks);

template
HDelay<int32_t>::HDelay(std::string id, HReader<int32_t>* reader, size_t blocksize, int blocks);

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