#ifndef __HAGC_CPP
#define __HAGC_CPP

#include <numeric>
#include <algorithm>

#include "hagc.h"

template <class T>
HAgc<T>::HAgc(std::string id, HWriter<T>* writer, T level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<T>* probe):
    HGain<T>(id, writer, 1, blocksize, probe),
    _level(level),
    _average(average),
    _averagePtr(0),
    _gain(1),
    _hold(0),
    _increment(increment),
    _allowUnity(allowUnity),
    _enabled(true) {
    HLog("HAgc(HWriter*, level=%d, average=%d, blocksize=%d, increment=%d, allowUnity=%d)", _level, _average, blocksize, _increment, _allowUnity);
    Init();
}

template <class T>
HAgc<T>::HAgc(std::string id, HWriterConsumer<T>* consumer, T level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<T>* probe):
    HGain<T>(id, consumer, 1, blocksize, probe),
    _level(level),
    _average(average),
    _averagePtr(0),
    _gain(1),
    _hold(0),
    _increment(increment),
    _allowUnity(allowUnity),
    _enabled(true) {
    HLog("HAgc(HWriterConsumer*, level=%d, average=%d, blocksize=%d, increment=%d, allowUnity=%d)", _level, _average, blocksize, _increment, _allowUnity);
    Init();
}

template <class T>
HAgc<T>::HAgc(std::string id, HReader<T>* reader, T level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<T>* probe):
    HGain<T>(id, reader, 1, blocksize, probe),
    _level(level),
    _average(average),
    _averagePtr(0),
    _gain(1),
    _hold(0),
    _increment(increment),
    _allowUnity(allowUnity),
    _enabled(true){
    HLog("HAgc(HReader*, level=%d, average=%d, blocksize=%d, increment=%d, allowUnity=%d)", _level, _average, blocksize, _increment, _allowUnity);
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
    if( _enabled ) {
        // Calculate new average
        T max = *std::max_element(src, src + blocksize);
        _averageBuffer[_averagePtr++] = max;
        _averagePtr = _averagePtr >= _average ? 0 : _averagePtr;

        if (++_hold > _average) {
            _hold = 0;

            int average = std::accumulate(_averageBuffer, _averageBuffer + _average, 0) / (int) _average;
            if (average != 0) {

                // Calculate diff between average and desired level
                float needed = ((float) _level / (float) average);
                if (needed >= 1 || (needed < 1 && !_allowUnity)) {
                    _gain += (needed - _gain) / _increment;
                    HGain<T>::SetGain(_gain);
                } else if (needed < 1 && _allowUnity) {
                    _gain = 1;
                    HGain<T>::SetGain(_gain);
                }
            }
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
HAgc<int8_t>::HAgc(std::string id, HWriter<int8_t>* writer, int8_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(std::string id, HWriter<uint8_t>* writer, uint8_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(std::string id, HWriter<int16_t>* writer, int16_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(std::string id, HWriter<int32_t>* writer, int32_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int32_t>* probe);

template
HAgc<int8_t>::HAgc(std::string id, HWriterConsumer<int8_t>* consumer, int8_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(std::string id, HWriterConsumer<uint8_t>* consumer, uint8_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(std::string id, HWriterConsumer<int16_t>* consumer, int16_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(std::string id, HWriterConsumer<int32_t>* consumer, int32_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int32_t>* probe);

template
HAgc<int8_t>::HAgc(std::string id, HReader<int8_t>* reader, int8_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int8_t>* probe);

template
HAgc<uint8_t>::HAgc(std::string id, HReader<uint8_t>* reader, uint8_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<uint8_t>* probe);

template
HAgc<int16_t>::HAgc(std::string id, HReader<int16_t>* reader, int16_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int16_t>* probe);

template
HAgc<int32_t>::HAgc(std::string id, HReader<int32_t>* reader, int32_t level, int average, size_t blocksize, int increment, bool allowUnity, HProbe<int32_t>* probe);

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