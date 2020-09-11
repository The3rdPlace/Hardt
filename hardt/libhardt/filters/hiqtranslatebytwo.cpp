#ifndef __HIQTRANSLATEBYTWO_CPP
#define __HIQTRANSLATEBYTWO_CPP

#include "hiqtranslatebytwo.h"

template <class T>
void HIqTranslateByTwo<T>::Filter(T* src, T* dest, size_t blocksize)
{
    /*
        (A, jB) * (C, jD)
        Re = (A * C) - (B * D)
        Im = ((A + B) * (C + D)) - (A * C) - (B * D)

        For the sequence [(1, j0), (-1, j0), (1, j0), ...] We get:

        Re = (A * [1|-1]) - (B * 0)  <=>
             [A|-A] - 0              <=>
             [A|-A]

        Im = ((A + B) * ([1|-1] + 0)) - (A * [1|-1]) - (B * 0)  <=>
             (A + B) * [1|-1]) - (A * [1|-1]) - 0               <=>
             ([1|-1] * A) + ([1|-1] * B) - A * [1|-1]           <=>
             [1|-1] * B
    */

    // Run translator
    int j = 1;
    for( int i = 0; i < blocksize; i += 2 )
    {
        dest[i] = src[i] * j;
        dest[i + 1] = src[i + 1] * j;
        j *= -1;
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Filter
template
void HIqTranslateByTwo<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HIqTranslateByTwo<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HIqTranslateByTwo<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HIqTranslateByTwo<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif