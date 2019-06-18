#ifndef __HFFT_CPP
#define __HFFT_CPP

#include "hfft.h"

#include <cmath>

#include <complex>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

template <class T>
HFft<T>::HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window):
    HOutput<T, HFftResults>(writer, size),
    _size(size),
    _average(average),
    _count(0),
    _window(window),
    _max(0)
{
    HLog("HFft(%d, %d, ...)", size, average);

    // Allocate a buffer for the spectrum and phase values
    _spectrum = new double[size];
    memset((void*) _spectrum, 0, size * sizeof(double));
    _phase = new double[size];
    memset((void*) _phase, 0, size * sizeof(double));
    _c = new Complex[size];
    memset((void*) _c, 0, size * sizeof(Complex));

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
int HFft<T>::Output(T* src, size_t size)
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
        _c[i] += x[i];
        if( std::abs(x[i]) > _max )
        {
            _max = std::abs(x[i]);
        }
    }

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        //_max = _max / _average;
        HLog("MAX %f", _max);
        // Calculate spectrum and phase
        for( int i = 0; i < N / 2; i++ )
        {
            // Get absolute value at point n
            double value = std::abs(_c[i]);

            // Spectrum values
            _spectrum[i] += value / _average;

            if( value > _max / 10 )
            {
                double tan = std::atan2( _c[i].imag(), _c[i].real() );
                double phase = (tan * 180) / M_PI;
                _phase[i] = phase;// / (double) _average;
            }
        }

        // Call the callback function with the calculated spectrum
        // Note that the FFT algorithm reverses the spectrum (even and odd is reversed)
        // so we need to return the first N/2 bins, not the second part of the spectrum
        HFftResults results;
        results.Spectrum = &_spectrum[0];
        results.Phase = &_phase[0];
        results.Size = N / 2;
        HOutput<T, HFftResults>::Ready(&results, 1);

        // Reset results
        _count = 0;
        memset((void*) _spectrum, 0, size * sizeof(double));
        memset((void*) _phase, 0, size * sizeof(double));
        memset((void*) _c, 0, size * sizeof(Complex));
        _max = 0;
    }

    // We took the entire window
    return size;
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
HFft<int8_t>::HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<int8_t>* window);

template
HFft<uint8_t>::HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<uint8_t>* window);

template
HFft<int16_t>::HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<int16_t>* window);

template
HFft<int32_t>::HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<int32_t>* window);

template
int HFft<int8_t>::Output(int8_t* src, size_t size);

template
int HFft<uint8_t>::Output(uint8_t* src, size_t size);

template
int HFft<int16_t>::Output(int16_t* src, size_t size);

template
int HFft<int32_t>::Output(int32_t* src, size_t size);
#endif
