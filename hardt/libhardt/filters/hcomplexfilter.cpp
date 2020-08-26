#ifndef __HCOMPLEXFILTER_CPP
#define __HCOMPLEXFILTER_CPP

#include "hcomplexfilter.h"

template <class T>
void HComplexFilter<T>::Filter(std::complex<T>* src, std::complex<T>* dest, size_t blocksize)
{
    // Run gain filter
    T r;
    T j;
    for( int i = 0; i < blocksize; i++ )
    {
        std::cout << "src real() " << std::to_string(src[i].real()) << ", imag() " << std::to_string(src[i].imag()) << std::endl;
        std::cout << "response real() " << std::to_string(_response[i].real()) << ", imag() " << std::to_string(_response[i].imag()) << std::endl;
        r = src[i].real() * _response[i].real() + src[i].imag() * _response[i].imag();
        j = src[i].real() * _response[i].imag() - src[i].imag() * _response[i].real();
        std::cout << "r " << std::to_string(r) << ", j " << std::to_string(j) << std::endl;
        dest[i] = std::complex<T>(r, j);
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Filter
template
void HComplexFilter<int8_t>::Filter(std::complex<int8_t>* src, std::complex<int8_t>* dest, size_t blocksize);

template
void HComplexFilter<uint8_t>::Filter(std::complex<uint8_t>* src, std::complex<uint8_t>* dest, size_t blocksize);

template
void HComplexFilter<int16_t>::Filter(std::complex<int16_t>* src, std::complex<int16_t>* dest, size_t blocksize);

template
void HComplexFilter<int32_t>::Filter(std::complex<int32_t>* src, std::complex<int32_t>* dest, size_t blocksize);

template
void HComplexFilter<float>::Filter(std::complex<float>* src, std::complex<float>* dest, size_t blocksize);

template
void HComplexFilter<double>::Filter(std::complex<double>* src, std::complex<double>* dest, size_t blocksize);

//! @endcond
#endif