#ifndef __HBUFFEREDWRITER_CPP
#define __HBUFFEREDWRITER_CPP

#include <cstdio>
#include <mutex>
#include <condition_variable>

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

    // Insert the block
    T* block = new T[blocksize];
    memcpy((void*) block, (void*) src, blocksize * sizeof(T));
    _buffer.push_back(block);

    // Notify a locked drain
    std::unique_lock<std::mutex> lck(_drainMutex);
    _drainLock.notify_one();

    return blocksize;
}

template <class T>
void HBufferedWriter<T>::Drain()
{
    // Wait untill we have data available
    if( _buffer.size() == 0 ) {
        std::unique_lock<std::mutex> lck(_drainMutex);
        _drainLock.wait(lck);
    }

    // Check if we got interrupted due to the drain thread being halted
    if( !_isDraining ) {
        return;
    }

    // Drain one block
    _writer->Write( _buffer.at(0), _blocksize);
    _buffer.erase( _buffer.begin(), _buffer.begin() + 1);

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

// Drain()
template
void HBufferedWriter<int8_t>::Drain();

template
void HBufferedWriter<uint8_t>::Drain();

template
void HBufferedWriter<int16_t>::Drain();

template
void HBufferedWriter<int32_t>::Drain();

//! @endcond
#endif