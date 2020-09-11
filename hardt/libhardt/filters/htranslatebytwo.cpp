#ifndef __HTRANSLATEBYTWO_CPP
#define __HTRANSLATEBYTWO_CPP

#include "htranslatebytwo.h"

template <class T>
void HTranslateByTwo<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Run translator
    int j = 1;
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = src[i] * j;
        j *= -1;
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Filter
template
void HTranslateByTwo<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HTranslateByTwo<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HTranslateByTwo<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HTranslateByTwo<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif