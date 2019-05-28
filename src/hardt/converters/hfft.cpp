#ifndef __HFFT_CPP
#define __HFFT_CPP

#include "hfft.h"

template <class T>
HFft<T>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback):
    HConverter<T, long int>(windowSize),
    _windowSize(windowSize),
    _bins(bins),
    _average(average),
    _callback(callback),
    _count(0)
{
    HLog("HFft(%d, %d, %d, ...)", windowSize, bins, average);

    // Allocate a buffer for the spectrum
    _spectrum = new long int[bins];
}

template <class T>
int HFft<T>::Convert(T* src, size_t windowSize)
{
    // Todo: Run the input buffer through a window function, if any is given

    // Todo: Calculate FFT of the input

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        // Todo: Call the callback function with the calculated spectrum
        _callback(_spectrum, _bins);

        // Reset results
        _count = 0;
    }

    // We took the entire window
    return windowSize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
HFft<int8_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback);

template
HFft<uint8_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback);

template
HFft<int16_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback);

template
HFft<int32_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback);

template
int HFft<int8_t>::Convert(int8_t* src, size_t windowSize);

template
int HFft<uint8_t>::Convert(uint8_t* src, size_t windowSize);

template
int HFft<int16_t>::Convert(int16_t* src, size_t windowSize);

template
int HFft<int32_t>::Convert(int32_t* src, size_t windowSize);
#endif
