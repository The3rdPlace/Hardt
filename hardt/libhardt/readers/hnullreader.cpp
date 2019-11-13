#ifndef __HNULLREADER_CPP
#define __HNULLREADER_CPP

#include "hnullreader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
int HNullReader<T>::Read(T* dest, size_t blocksize)
{
    for( int i = 0; i < blocksize; i++ ) 
    {
        dest[i] = _zero;
    }

    this->Metrics.Reads++;
    this->Metrics.BlocksIn++;
    this->Metrics.BytesIn += blocksize * sizeof(T);

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Read()
template
int HNullReader<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HNullReader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HNullReader<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HNullReader<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
