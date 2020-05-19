#ifndef __HBUFFEREDWRITER_CPP
#define __HBUFFEREDWRITER_CPP

#include <cstdio>
#include <mutex>
#include <condition_variable>

#include "hbufferedwriter.h"

template <class T>
int HBufferedWriter<T>::Write(T* src, size_t blocksize)
{
    // Check if buffering is disabled
    if( !_enabled )
    {
        return _writer->Write(src, blocksize);
    }

    // Check the blocksize
    if( blocksize != _blocksize ) {
        throw new HWriterIOException("Invalid blocksize for write to HBufferedWriter");
    }

    // --------------------------------------------------------------------------------------
    // ENTER CRITICAL SECTION: Access to the buffer
    std::unique_lock<std::mutex> lck (_readWriteMutex, std::defer_lock);
    lck.lock();

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

    lck.unlock();
    // EXIT CRITICAL_SECTION
    // --------------------------------------------------------------------------------------

    // Notify a locked drain
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

    // --------------------------------------------------------------------------------------
    // ENTER CRITICAL SECTION: Access to the buffer
    std::unique_lock<std::mutex> lck (_readWriteMutex, std::defer_lock);
    lck.lock();

    // Drain one block
    T* src = _buffer.at(0);
    _writer->Write(src, _blocksize);
    delete[] src;
    _buffer.erase( _buffer.begin(), _buffer.begin() + 1);

    lck.unlock();
    // EXIT CRITICAL_SECTION
    // --------------------------------------------------------------------------------------
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