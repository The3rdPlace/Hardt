#ifndef __HCONVERTER_CPP
#define __HCONVERTER_CPP

#include "hcomplexconverter.h"

template<>
int HComplexConverter<int8_t, std::complex<int8_t>>::Convert(int8_t * src, std::complex<int8_t>* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = std::complex<int8_t>(src[i], 0);
    }
    return blocksize;
}

template<>
int HComplexConverter<int16_t, std::complex<int16_t >>::Convert(int16_t * src, std::complex<int16_t >* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = std::complex<int16_t >(src[i], 0);
    }
    return blocksize;
}

template<>
int HComplexConverter<int32_t, std::complex<int32_t >>::Convert(int32_t * src, std::complex<int32_t >* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = std::complex<int32_t >(src[i], 0);
    }
    return blocksize;
}

template<>
int HComplexConverter<float, std::complex<float>>::Convert(float * src, std::complex<float>* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = std::complex<float>(src[i], 0);
    }
    return blocksize;
}

template<>
int HComplexConverter<double, std::complex<double>>::Convert(double * src, std::complex<double>* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = std::complex<double>(src[i], 0);
    }
    return blocksize;
}

template<>
int HComplexConverter<std::complex<int8_t>, int8_t>::Convert(std::complex<int8_t> * src, int8_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int8_t) std::abs(src[i]);
    }
    return blocksize;
}

template<>
int HComplexConverter<std::complex<int16_t>, int16_t>::Convert(std::complex<int16_t> * src, int16_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int16_t) std::abs(src[i]);
    }
    return blocksize;
}

template<>
int HComplexConverter<std::complex<int32_t>, int32_t>::Convert(std::complex<int32_t> * src, int32_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int32_t) std::abs(src[i]);
    }
    return blocksize;
}

template<>
int HComplexConverter<std::complex<float>, float>::Convert(std::complex<float> * src, float* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (float) std::abs(src[i]);
    }
    return blocksize;
}

template<>
int HComplexConverter<std::complex<double>, double>::Convert(std::complex<double> * src, double* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (double) std::abs(src[i]);
    }
    return blocksize;
}

#endif
