#ifndef __HGAUSSIANFILTER_CPP
#define __HGAUSSIANFILTER_CPP

#include "hgaussianfilter.h"

template <class T>
void HGaussianFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    for( int i = 0; i < _blocks; i++ ) {

        // Take fft of block
        _fft->FFT(&src[i * _n], _response);

        // Find instantaneous signal level and set a cutoff level
        double avg = 0;
        for( int j = 0; j < _n; j++ ) {
            avg += std::abs(_response[j]);
        }
        avg /= _n;
        avg *= _strength;

        // Filter out low-energy spectral bins
        for( int j = 0; j < _n; j++ ) {
            if( std::abs(_response[j]) < avg ) {
                _response[j] = std::complex<double>(0, 0);
            }
        }

        // Take ifft to get the real signal back
        _fft->IFFT(_response, &dest[i * _n]);
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Filter
template
void HGaussianFilter<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HGaussianFilter<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HGaussianFilter<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HGaussianFilter<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif