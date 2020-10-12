#ifndef __HIQTRANSLATEBYFOUR_CPP
#define __HIQTRANSLATEBYFOUR_CPP

#include "hiqtranslatebyfour.h"

template <class T>
void HIqTranslateByFour<T>::DoFilter(T* src, T* dest, size_t blocksize, bool isQ)
{
    /*
        (A, jB) * (C, jD)
        Re = (A * C) - (B * D)
        Im = ((A + B) * (C + D)) - (A * C) - (B * D)

        For the sequence [(1, j0), (0, j0), (-1, j0), (0, j0), ...] We get:

        Re = (A * [1|0|-1|0]) - (B * 0)  <=>
             [A|0|-A|0] - 0              <=>
             [A|0|-A|0]

        Im = ((A + B) * ([1|0|-1|0] + 0)) - (A * [1|0|-1|0]) - (B * 0)  <=>
             (A + B) * [1|0|-1|0]) - (A * [1|0|-1|0]) - 0               <=>
             ([1|0|-1|0] * A) + ([1|0|-1|0] * B) - A * [1|0|-1|0]       <=>
             [1|0|-1|0] * B                                             <=>
             B|0|-B|0
    */

    // Setup translation
    int IFactor[4] = {1, 0, -1, 0};
    int QFactor[4] = {0, -1, 0, 1};
    int *factors = isQ ? QFactor : IFactor;
    int factor = 0;

    // Run translator
    for( int i = 0; i < blocksize; i += 2 )
    {
        dest[i] = src[i] * factors[factor];
        dest[i + 1] = src[i + 1] * factors[factor];
        factor++;

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
void HIqTranslateByFour<int8_t>::DoFilter(int8_t* src, int8_t* dest, size_t blocksize, bool isQ);

template
void HIqTranslateByFour<uint8_t>::DoFilter(uint8_t* src, uint8_t* dest, size_t blocksize, bool isQ);

template
void HIqTranslateByFour<int16_t>::DoFilter(int16_t* src, int16_t* dest, size_t blocksize, bool isQ);

template
void HIqTranslateByFour<int32_t>::DoFilter(int32_t* src, int32_t* dest, size_t blocksize, bool isQ);

//! @endcond
#endif