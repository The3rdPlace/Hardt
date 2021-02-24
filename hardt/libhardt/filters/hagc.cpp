#ifndef __HAGC_CPP
#define __HAGC_CPP

#include <numeric>
#include <algorithm>

#include "hagc.h"

template <class T>
HAgc<T>::HAgc(HWriter<T>* writer, T level, int average, size_t blocksize, HProbe<T>* probe):
    HGain<T>(writer, 1, blocksize, probe),
    _level(level),
    _average(average),
    _averagePtr(0),
    _gain(1),
    _hold(0)
{
    HLog("HAgc(HWriter*, %d, %d)", level, blocksize);
    Init();
}

template <class T>
HAgc<T>::HAgc(HWriterConsumer<T>* consumer, T level, int average, size_t blocksize, HProbe<T>* probe):
    HGain<T>(consumer, 1, blocksize, probe),
    _level(level),
    _average(average),
    _averagePtr(0),
    _gain(1),
    _hold(0)
{
    HLog("HAgc(HWriterConsumer*, %d, %d)", level, blocksize);
    Init();
}

template <class T>
HAgc<T>::HAgc(HReader<T>* reader, T level, int average, size_t blocksize, HProbe<T>* probe):
    HGain<T>(reader, 1, blocksize, probe),
    _level(level),
    _average(average),
    _averagePtr(0),
    _gain(1),
    _hold(0)
{
    HLog("HAgc(HReader*, %d, %d)", level, blocksize);
    Init();
}

template <class T>
HAgc<T>::~HAgc()
{
    HLog("~HAgc()");
    delete _averageBuffer;
}

template <class T>
void HAgc<T>::Filter(T* src, T* dest, size_t blocksize)
{
    if( ++_hold > _average) {
        _hold = 0;

        // Calculate new average
        T max = *std::max_element(dest, dest + blocksize);
        _averageBuffer[_averagePtr++] = max;
        _averagePtr = _averagePtr >= _average ? 0 : _averagePtr;
        int average = std::accumulate(_averageBuffer, _averageBuffer + _average, 0) / (int) _average;

        if (average != 0) {

            // Calculate diff between average and desired level
            _gain = 1 / ((float) average / (float) _level);
            HGain<T>::SetGain(_gain);
        }
    }

    // Filter the block (apply gain)
    HGain<T>::Filter(src, dest, blocksize);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HAgc
template
HAgc<int8_t>::HAgc(HWriter<int8_t>* writer, int8_t level, int average, size_t blocksize, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(HWriter<uint8_t>* writer, uint8_t level, int average, size_t blocksize, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(HWriter<int16_t>* writer, int16_t level, int average, size_t blocksize, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(HWriter<int32_t>* writer, int32_t level, int average, size_t blocksize, HProbe<int32_t>* probe);

template
HAgc<int8_t>::HAgc(HWriterConsumer<int8_t>* consumer, int8_t level, int average, size_t blocksize, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(HWriterConsumer<uint8_t>* consumer, uint8_t level, int average, size_t blocksize, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(HWriterConsumer<int16_t>* consumer, int16_t level, int average, size_t blocksize, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(HWriterConsumer<int32_t>* consumer, int32_t level, int average, size_t blocksize, HProbe<int32_t>* probe);

template
HAgc<int8_t>::HAgc(HReader<int8_t>* reader, int8_t level, int average, size_t blocksize, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(HReader<uint8_t>* reader, uint8_t level, int average, size_t blocksize, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(HReader<int16_t>* reader, int16_t level, int average, size_t blocksize, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(HReader<int32_t>* reader, int32_t level, int average, size_t blocksize, HProbe<int32_t>* probe);

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