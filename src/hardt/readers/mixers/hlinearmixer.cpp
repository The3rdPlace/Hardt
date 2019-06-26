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

    _buffer = new T[blocksize];
    HLog("Allocated %d as local buffer", blocksize * sizeof(T));
}

template <class T>
HLinearMixer<T>::~HLinearMixer()
{
    HLog("~HLinearMixer()");
    delete _buffer;
}

template <class T>
int HLinearMixer<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HlinearMixer. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation of the mixer");
    }

    // Todo: read from both inputs
    int received = _reader_1->Read(dest, blocksize);

    return received;
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