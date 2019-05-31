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


void runFFT(double* data, unsigned long nn)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;

    // reverse-binary reindexing
    n = nn<<1;
    j=1;
    for (i=1; i<n; i+=2) {
        HLog("--i=%d--j=%d", i, j);
        if (j>i) {
            HLog("swap(%d, %d)", j-i, i-1);
            std::swap(data[j-1], data[i-1]);
            HLog("swap(%d, %d)", j, i);
            std::swap(data[j], data[i]);
        }
        m = nn;
        while (m>=2 && j>m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };

    // here begins the Danielson-Lanczos section
    mmax=2;
    while (n>mmax) {
        istep = mmax<<1;
        theta = -(2*M_PI/mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m=1; m < mmax; m += 2) {
            for (i=m; i <= n; i += istep) {
                j=i+mmax;
                tempr = wr*data[j-1] - wi*data[j];
                tempi = wr * data[j] + wi*data[j-1];

                //data[j-1] = data[i-1] - tempr;
                data[j] = data[i] - tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wtemp=wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
        }
        mmax=istep;
    }

}



template <class T>
int HFft<T>::Convert(T* src, size_t windowSize)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, windowSize);

    // Todo: Calculate FFT of the input
    double input[16];
    //memcpy((void*) input, (void*) src, windowSize * sizeof(double));
    //memset((void*) &input[windowSize], 0, windowSize * sizeof(double));
    input[0] = 1;
    input[1] = 2;
    input[2] = 3;
    input[3] = 4;
    input[4] = 5;
    input[5] = 6;
    input[6] = 7;
    input[7] = 8;
    input[8] = 0;
    input[9] = 0;
    input[10] = 0;
    input[11] = 0;
    input[12] = 0;
    input[13] = 0;
    input[14] = 0;
    input[15] = 0;

    runFFT(input, 8);

    for( int i = 0; i < 16; i++ )
    {
        HLog("i=%d  %d", i, (int) input[i]);
    }

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
