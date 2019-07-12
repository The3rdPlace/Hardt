#ifndef __HGAIN_CPP
#define __HGAIN_CPP

#include "hwriter.h"
#include "hreader.h"
#include "hgain.h"

template <class T>
HGain<T>::HGain(HWriter<T>* writer, float gain, size_t blocksize):
    _writer(writer),
    _reader(NULL),
    _blocksize(blocksize),
    _gain(gain)
{
    HLog("HGain(HWriter*, %f, %d)", gain, blocksize);

    _buffer = new T[blocksize];
    HLog("Allocated %d as local buffer", blocksize * sizeof(T));
}

template <class T>
HGain<T>::HGain(HReader<T>* reader, float gain, size_t blocksize):
    _writer(NULL),
    _reader(reader),
    _blocksize(blocksize),
    _gain(gain)
{
    HLog("HGain(HReader*, %f, %d)", gain, blocksize);

    _buffer = new T[blocksize];
    HLog("Allocated %d as local buffer", blocksize * sizeof(T));
}

template <class T>
HGain<T>::~HGain()
{
    HLog("~HGain()");
    delete _buffer;
}

template <class T>
int HGain<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HGain. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not allowed to write more data than the size given at creation");
    }

    for( int i = 0; i < blocksize; i++ )
    {
        _buffer[i] = _gain * src[i];
    }
    return _writer->Write(_buffer, blocksize);
}

template <class T>
int HGain<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HGain. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation");
    }

    int received = _reader->Read(_buffer, blocksize);
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = _gain * _buffer[i];
    }
    return received;
}

template <class T>
void HGain<T>::SetGain(float gain)
{
    _gain = gain;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HGain
template
HGain<int8_t>::HGain(HWriter<int8_t>* writer, float gain, size_t blocksize);

template
HGain<uint8_t>::HGain(HWriter<uint8_t>* writer, float gain, size_t blocksize);

template
HGain<int16_t>::HGain(HWriter<int16_t>* writer, float gain, size_t blocksize);

template
HGain<int32_t>::HGain(HWriter<int32_t>* writer, float gain, size_t blocksize);

template
HGain<int8_t>::HGain(HReader<int8_t>* reader, float gain, size_t blocksize);

template
HGain<uint8_t>::HGain(HReader<uint8_t>* reader, float gain, size_t blocksize);

template
HGain<int16_t>::HGain(HReader<int16_t>* reader, float gain, size_t blocksize);

template
HGain<int32_t>::HGain(HReader<int32_t>* reader, float gain, size_t blocksize);

// ~HGain()
template
HGain<int8_t>::~HGain();

template
HGain<uint8_t>::~HGain();

template
HGain<int16_t>::~HGain();

template
HGain<int32_t>::~HGain();

// Write()
template
int HGain<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HGain<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HGain<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HGain<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HGain<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HGain<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HGain<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HGain<int32_t>::Read(int32_t* dest, size_t blocksize);

// SetGain()
template
void HGain<int8_t>::SetGain(float gain);

template
void HGain<uint8_t>::SetGain(float gain);

template
void HGain<int16_t>::SetGain(float gain);

template
void HGain<int32_t>::SetGain(float gain);

#endif