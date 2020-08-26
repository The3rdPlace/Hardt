#ifndef __HDECIMATEBYFOUR_CPP
#define __HDECIMATEBYFOUR_CPP

#include "htranslatebyfour.h"

template <class T>
void HTranslateByFour<T>::DoFilter(T* src, T* dest, size_t blocksize, bool isQ)
{
    // Setup translation
    int IFactor[4] = {1, 0, -1, 0};
    int QFactor[4] = {0, -1, 0, 1};
    int *factors = isQ ? QFactor : IFactor;
    int factor = 0;

    // Run translator
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = src[i] * factors[factor++];
        if( factor > 3 ) {
            factor = 0;
        }
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Filter
template
void HTranslateByFour<int8_t>::DoFilter(int8_t* src, int8_t* dest, size_t blocksize, bool isQ);

template
void HTranslateByFour<uint8_t>::DoFilter(uint8_t* src, uint8_t* dest, size_t blocksize, bool isQ);

template
void HTranslateByFour<int16_t>::DoFilter(int16_t* src, int16_t* dest, size_t blocksize, bool isQ);

template
void HTranslateByFour<int32_t>::DoFilter(int32_t* src, int32_t* dest, size_t blocksize, bool isQ);

//! @endcond
#endif