#ifndef __HNULLREADER_CPP
#define __HNULLREADER_CPP

#include "hnullreader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
int HNullReader<T>::ReadImpl(T* dest, size_t blocksize)
{
    for( int i = 0; i < blocksize; i++ ) 
    {
        dest[i] = _zero;
    }

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Read()
template
int HNullReader<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HNullReader<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HNullReader<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HNullReader<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif
