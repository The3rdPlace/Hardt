#ifndef __HTYPECONVERTER_CPP
#define __HTYPECONVERTER_CPP

#include "htypeconverter.h"

template<>
int HTypeConverter<int8_t, int16_t>::Convert(int8_t * src, int16_t * dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((int16_t) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int8_t, int32_t>::Convert(int8_t * src, int32_t * dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((int32_t) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int8_t , float>::Convert(int8_t * src, float* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((float) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int8_t , double>::Convert(int8_t * src, double* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((double) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t, int32_t>::Convert(int16_t * src, int32_t * dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((int32_t) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t , float>::Convert(int16_t * src, float* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((float) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t , double>::Convert(int16_t * src, double* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((double) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<int32_t , double>::Convert(int32_t * src, double* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = ((double) src[i]) * _scale;
    }
    return blocksize;
}

template<>
int HTypeConverter<float, int8_t>::Convert(float * src, int8_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int8_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<float, uint8_t>::Convert(float * src, uint8_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (uint8_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<float, int16_t>::Convert(float * src, int16_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int16_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<float, int32_t>::Convert(float * src, int32_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int16_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<double, int32_t>::Convert(double * src, int32_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int32_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<int32_t, int16_t>::Convert(int32_t * src, int16_t * dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int16_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<int32_t, int8_t>::Convert(int32_t * src, int8_t * dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int8_t) (src[i] * _scale);
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t, int8_t>::Convert(int16_t * src, int8_t * dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int8_t) (src[i] * _scale);
    }
    return blocksize;
}

#endif
