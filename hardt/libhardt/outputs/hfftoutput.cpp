#ifndef __HFFTOUTPUT_CPP
#define __HFFTOUTPUT_CPP

#include "hfftoutput.h"

#include <cmath>

#include <complex>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

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
    _spectrum = new double[_size];
    memset((void*) _spectrum, 0, _size * sizeof(double));
    _result = new Complex[_size];
    memset((void*) _result, 0, _size * sizeof(Complex));

    // Allocate a buffer for intermediate results
    _buffer = new T[_size];

    // Set window size
    _window->SetSize(_size);
}

void fft(CArray& x)
{

    const size_t N = x.size();
    if (N <= 1) return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

template <class T>
int HFftOutput<T>::Output(T* src, size_t size)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, size);

    // Prepare a set of complex values
    int N = size;
    CArray x(N);
    for( int i = 0; i < N ; i++ )
    {
        Complex r(_buffer[i], 0);
        x[i] = r;
    }

    // Calculate the FFT
    fft(x);

    // Accumulate the results for averaging later
    for( int i = 0; i < N / 2; i++ )
    {
        _result[i] += x[i];
    }

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        // Calculate spectrum
        for( int i = 0; i < N / 2; i++ )
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
        results.Size = N / 2;
        HOutput<T, HFftResults>::Ready(&results, 1);

        // Reset results
        _count = 0;
        memset((void*) _spectrum, 0, size * sizeof(double));
        memset((void*) _result, 0, size * sizeof(Complex));
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
