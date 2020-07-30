#ifndef __HTYPECONVERTER_CPP
#define __HTYPECONVERTER_CPP

#include "htypeconverter.h"

template<>
int HTypeConverter<int8_t, int16_t>::Convert(int8_t * src, int16_t * dest, size_t blocksize) {
    const int16_t FACTOR = 258;  // 32.767 / 127
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = _scale
                ? ((int16_t) src[i]) * FACTOR
                : (int16_t) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int8_t, int32_t>::Convert(int8_t * src, int32_t * dest, size_t blocksize) {
    const int32_t FACTOR = 16909320;  // 214.748.3647 / 127
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = _scale
                  ? ((int32_t) src[i]) * FACTOR
                  : (int32_t) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t, int32_t>::Convert(int16_t * src, int32_t * dest, size_t blocksize) {
    const int32_t FACTOR = 65537;  // 214.748.3647 / 32767
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = _scale
                  ? ((int32_t) src[i]) * FACTOR
                  : (int32_t) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int8_t , float>::Convert(int8_t * src, float* dest, size_t blocksize) {
    const float FACTOR = 258;  // 32.767 / 127
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = _scale
                  ? ((float) src[i]) * FACTOR
                  : (float) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t , float>::Convert(int16_t * src, float* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (float) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int8_t , double>::Convert(int8_t * src, double* dest, size_t blocksize) {
    const double FACTOR = 16909320;  // 214.748.3647 / 127
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = _scale
                  ? ((double) src[i]) * FACTOR
                  : (double) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t , double>::Convert(int16_t * src, double* dest, size_t blocksize) {
    const double FACTOR = 65537;  // 214.748.3647 / 32767
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = _scale
                  ? ((double) src[i]) * FACTOR
                  : (double) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int32_t , double>::Convert(int32_t * src, double* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (double) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<float, int16_t>::Convert(float * src, int16_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (float) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<double, int32_t>::Convert(double * src, int32_t* dest, size_t blocksize) {
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int32_t) src[i];
    }
    return blocksize;
}

template<>
int HTypeConverter<int32_t, int16_t>::Convert(int32_t * src, int16_t * dest, size_t blocksize) {
    if( !_scale ) {
        HError("Not converting from int32 to int16 without scaling");
        throw new HConverterIOException("Not converting without scaling");
    }

    const int32_t FACTOR = 65537;  // 214.748.3647 / 32767
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int16_t) (src[i] / FACTOR);
    }
    return blocksize;
}

template<>
int HTypeConverter<int32_t, int8_t>::Convert(int32_t * src, int8_t * dest, size_t blocksize) {
    if( !_scale ) {
        HError("Not converting from int32 to int8 without scaling");
        throw new HConverterIOException("Not converting without scaling");
    }

    const int32_t FACTOR = 16909320;  // 214.748.3647 / 127
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int8_t) (src[i] / FACTOR);
    }
    return blocksize;
}

template<>
int HTypeConverter<int16_t, int8_t>::Convert(int16_t * src, int8_t * dest, size_t blocksize) {
    if( !_scale ) {
        HError("Not converting from int16 to int8 without scaling");
        throw new HConverterIOException("Not converting without scaling");
    }

    const int16_t FACTOR = 258;  // 32.767 / 127
    for(int i = 0; i < blocksize; i++ ) {
        dest[i] = (int8_t) (src[i] / FACTOR);
    }
    return blocksize;
}

#endif
