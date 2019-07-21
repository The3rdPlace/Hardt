#ifndef __HSPLITTER_CPP
#define __HSPLITTER_CPP

#include "hsplitter.h"

template <class T>
int HSplitter<T>::Write(T* src, size_t blocksize)
{
    int written = _writer1->Write(src, blocksize);
    if( written != blocksize )
    {
        HError("Incorrect write to writer 1 (%d blocks)", written);
        return 0;
    }
    written = _writer2->Write(src, blocksize);
    if( written != blocksize )
    {
        HError("Incorrect write to writer 2 (%d blocks)", written);
        return 0;
    }
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Write()
template
int HSplitter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HSplitter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HSplitter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HSplitter<int32_t>::Write(int32_t* src, size_t blocksize);

#endif