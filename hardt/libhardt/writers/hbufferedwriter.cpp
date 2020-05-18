#ifndef __HBUFFEREDWRITER_CPP
#define __HBUFFEREDWRITER_CPP

#include "hbufferedwriter.h"

template <class T>
int HBufferedWriter<T>::Write(T* src, size_t blocksize)
{
    // Check capacity
    if( _buffer.size() == _blocks )
    {
        HLog("Reserve more space in buffer %d -> %d", _blocks, _blocks + _blocksReserved);
        _blocks += _blocksReserved;
        _buffer.reserve(_blocks);
    }

    T* block = new T[blocksize];
    memcpy((void*) block, (void*) src, blocksize * sizeof(T));
    _buffer.push_back(block);

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Write()
template
int HBufferedWriter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HBufferedWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HBufferedWriter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HBufferedWriter<int32_t>::Write(int32_t* src, size_t blocksize);

//! @endcond
#endif