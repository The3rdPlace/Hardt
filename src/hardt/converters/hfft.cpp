#ifndef __HFFT_CPP
#define __HFFT_CPP

#include "hfft.h"

#include <math.h>

#include <complex>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

template <class T>
HFft<T>::HFft(int size, int average, HWriter<long int>* writer, HWindow<T>* window):
    HConverter<T, long int>(writer, size),
    _size(size),
    _average(average),
    _count(0),
    _window(window)
{
    HLog("HFft(%d, %d, ...)", size, average);

    // Allocate a buffer for the spectrum and phase values
    _spectrum = new long int[size];
    memset((void*) _spectrum, 0, size * sizeof(long int));
    _phase = new double[size];
    memset((void*) _phase, 0, size * sizeof(double));

    // Allocate a buffer for intermediate results
    _buffer = new T[size];

    // Set window size
    _window->SetSize(size);
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
int HFft<T>::Convert(T* src, size_t size)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, size);

    int N = size;

    CArray x(N);
    for( int i = 0; i < N ; i++ )
    {
        Complex r(src[i], 0);
        x[i] = r;
    }
    fft(x);

    for( int i = 0; i < N; i++ )
    {
        _spectrum[i] += std::abs(x[i]) / _average;
        _phase[i] += std::atan( x[i].imag() / x[i].real() ) / _average;
    }

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        // Call the callback function with the calculated spectrum
        HConverter<T, long int>::Ready(_spectrum, N / 2);

        // Reset results
        _count = 0;
        memset((void*) _spectrum, 0, size * sizeof(long int));
        memset((void*) _phase, 0, size * sizeof(double));
    }

    // We took the entire window
    return size;
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
HFft<int8_t>::HFft(int size, int average, HWriter<long int>* writer, HWindow<int8_t>* window);

template
HFft<uint8_t>::HFft(int size, int average, HWriter<long int>* writer, HWindow<uint8_t>* window);

template
HFft<int16_t>::HFft(int size, int average, HWriter<long int>* writer, HWindow<int16_t>* window);

template
HFft<int32_t>::HFft(int size, int average, HWriter<long int>* writer, HWindow<int32_t>* window);

template
int HFft<int8_t>::Convert(int8_t* src, size_t size);

template
int HFft<uint8_t>::Convert(uint8_t* src, size_t size);

template
int HFft<int16_t>::Convert(int16_t* src, size_t size);

template
int HFft<int32_t>::Convert(int32_t* src, size_t size);
#endif
