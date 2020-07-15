#ifndef __HFFTOUTPUT_CPP
#define __HFFTOUTPUT_CPP

#include "hwriter.h"
#include "hwriterconsumer.h"

#include "hfftoutput.h"

#include <cmath>
#include <complex>
#include <valarray>

template <class T>
HFftOutput<T>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window):
    HOutput<T, HFftResults>(writer, size),
    _size(size),
    _average(average),
    _count(0),
    _window(window)
{
    HLog("HFftOutput(%d, %d, ...)", size, average);
    Init();
}

template <class T>
HFftOutput<T>::HFftOutput(int size, int average, HWriterConsumer<T>* consumer, HWindow<T>* window):
    HOutput<T, HFftResults>(size, consumer),
    _size(size),
    _average(average),
    _count(0),
    _window(window)
{
    HLog("HFftOutput(%d, %d, ...)", size, average);
    Init();
}

template <class T>
void HFftOutput<T>::Init()
{
    // Allocate a buffer for the spectrum and phase values
    // Do not forget to zero the result buffer as new results are being added to the 
    // current values, not overriding them!
    _spectrum = new double[_size];
    memset((void*) _spectrum, 0, _size * sizeof(double));
    _result = new std::complex<double>[_size];
    memset((void*) _result, 0, _size * sizeof(std::complex<double>));

    // Allocate a buffer for intermediate results
    _buffer = new T[_size];

    // Set window size
    _window->SetSize(_size);

    // Create the fft
    _fft = new HFft<T>(_size, _window);
}


template <class T>
int HFftOutput<T>::Output(T* src, size_t size)
{
    _fft->FFT(src, _result);

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        // Calculate spectrum
        for( int i = 0; i < size / 2; i++ )
        {
            // Get absolute value at point n
            double value = std::abs(_result[i]);

            // Spectrum values
            _spectrum[i] += value / _average;
        }

        // Call the callback function with the calculated spectrum
        // Note that this FFT algorithm reverses the spectrum (even and odd is reversed)
        // so we need to return the first N/2 bins, not the second part of the spectrum
        HFftResults results;
        results.Spectrum = &_spectrum[0];
        results.Result = &_result[0];
        results.Size = size / 2;
        HOutput<T, HFftResults>::Ready(&results, 1);

        // Reset results
        _count = 0;
        memset((void*) _spectrum, 0, size * sizeof(double));
        memset((void*) _result, 0, size * sizeof(std::complex<double>));
    }

    // We took the entire window
    return size;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

template
HFftOutput<int8_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<int8_t>* window);

template
HFftOutput<uint8_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<uint8_t>* window);

template
HFftOutput<int16_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<int16_t>* window);

template
HFftOutput<int32_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<int32_t>* window);

template
HFftOutput<int8_t>::HFftOutput(int size, int average, HWriterConsumer<int8_t>* consumer, HWindow<int8_t>* window);

template
HFftOutput<uint8_t>::HFftOutput(int size, int average, HWriterConsumer<uint8_t>* consumer, HWindow<uint8_t>* window);

template
HFftOutput<int16_t>::HFftOutput(int size, int average, HWriterConsumer<int16_t>* consumer, HWindow<int16_t>* window);

template
HFftOutput<int32_t>::HFftOutput(int size, int average, HWriterConsumer<int32_t>* consumer, HWindow<int32_t>* window);

template
int HFftOutput<int8_t>::Output(int8_t* src, size_t size);

template
int HFftOutput<uint8_t>::Output(uint8_t* src, size_t size);

template
int HFftOutput<int16_t>::Output(int16_t* src, size_t size);

template
int HFftOutput<int32_t>::Output(int32_t* src, size_t size);

//! @endcond
#endif
