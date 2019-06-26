#ifndef __HLINEARMIXER_CPP
#define __HLINEARMIXER_CPP

#include "hreader.h"
#include "hlinearmixer.h"

template <class T>
HLinearMixer<T>::HLinearMixer(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize):
    _reader_1(reader_1),
    _reader_2(reader_2),
    _blocksize(blocksize)
{
    HLog("Hlinearmixer(HReader*, HReader*)");
    _buffer_1 = new T[blocksize];
    _buffer_2 = new T[blocksize];
    HLog("Allocated 2 X %d as local buffers", blocksize * sizeof(T));
}

template <class T>
HLinearMixer<T>::~HLinearMixer()
{
    HLog("~HLinearMixer()");
    delete _buffer_1;
    delete _buffer_2;
}

template <class T>
int HLinearMixer<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HlinearMixer. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation of the mixer");
    }

    // Read from both inputs
    int received_1 = _reader_1->Read(_buffer_1, blocksize);
    int received_2 = _reader_2->Read(_buffer_2, blocksize);
    if( received_1 <= 0)
    {
        HLog("Zero read from reader-1, stopping");
        return 0;
    }
    if( received_2 <= 0)
    {
        HLog("Zero read from reader-2, stopping");
        return 0;
    }
    if( received_1 != received_2 )
    {
        HError("Short read from reader 1 or 2, returning empty frame");
        memset((void*) dest, 0, sizeof(T) * blocksize);
        return blocksize;
    }

    // Mix inputs
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = _buffer_1[i] + _buffer_2[i];
    }
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HLinearMixer
template
HLinearMixer<int8_t>::HLinearMixer(HReader<int8_t>* reader_1, HReader<int8_t>* reader_2, size_t blocksize);

template
HLinearMixer<uint8_t>::HLinearMixer(HReader<uint8_t>* reader_1, HReader<uint8_t>* reader_2, size_t blocksize);

template
HLinearMixer<int16_t>::HLinearMixer(HReader<int16_t>* reader_1, HReader<int16_t>* reader_2, size_t blocksize);

template
HLinearMixer<int32_t>::HLinearMixer(HReader<int32_t>* reader_1, HReader<int32_t>* reader_2, size_t blocksize);

// ~HLinearMixer()
template
HLinearMixer<int8_t>::~HLinearMixer();

template
HLinearMixer<uint8_t>::~HLinearMixer();

template
HLinearMixer<int16_t>::~HLinearMixer();

template
HLinearMixer<int32_t>::~HLinearMixer();

// Read()
template
int HLinearMixer<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HLinearMixer<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HLinearMixer<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HLinearMixer<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif