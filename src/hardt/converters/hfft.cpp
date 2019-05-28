#ifndef __HFFT_CPP
#define __HFFT_CPP

#include "hfft.h"

template <class T>
HFft<T>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback, HWindow<T>* window):
    HConverter<T, long int>(windowSize),
    _windowSize(windowSize),
    _bins(bins),
    _average(average),
    _callback(callback),
    _count(0),
    _window(window)
{
    HLog("HFft(%d, %d, %d, ...)", windowSize, bins, average);

    // Allocate a buffer for the spectrum
    _spectrum = new long int[bins];
    _spectrum = 0;

    // Allocate a buffer for intermediate results
    _buffer = new T[bins];

    // Set window size
    _window->SetSize(windowSize);
}

template <class T>
int HFft<T>::Convert(T* src, size_t windowSize)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, windowSize);

    // Todo: Calculate FFT of the input

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        // Call the callback function with the calculated spectrum
        _callback(_spectrum, _bins);

        // Reset results
        _count = 0;
        _spectrum = 0;
    }

    // We took the entire window
    return windowSize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
HFft<int8_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback, HWindow<int8_t>* window);

template
HFft<uint8_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback, HWindow<uint8_t>* window);

template
HFft<int16_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback, HWindow<int16_t>* window);

template
HFft<int32_t>::HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback, HWindow<int32_t>* window);

template
int HFft<int8_t>::Convert(int8_t* src, size_t windowSize);

template
int HFft<uint8_t>::Convert(uint8_t* src, size_t windowSize);

template
int HFft<int16_t>::Convert(int16_t* src, size_t windowSize);

template
int HFft<int32_t>::Convert(int32_t* src, size_t windowSize);
#endif
