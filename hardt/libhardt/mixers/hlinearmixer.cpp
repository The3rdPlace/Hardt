#ifndef __HLINEARMIXER_CPP
#define __HLINEARMIXER_CPP

#include "hreader.h"
#include "hlinearmixer.h"

template <class T>
HLinearMixer<T>::HLinearMixer(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize):
    _reader_1(reader_1),
    _reader_2(reader_2),
    _writer(NULL),
    _blocksize(blocksize)
{
    HLog("Hlinearmixer(HReader*, HReader*)");
    Init(blocksize);
}

template <class T>
HLinearMixer<T>::HLinearMixer(HReader<T>* reader, HWriter<T>* writer, size_t blocksize):
    _reader_1(reader),
    _reader_2(NULL),
    _writer(writer),
    _blocksize(blocksize)
{
    HLog("Hlinearmixer(HReader*, HWriter*)");
    Init(blocksize);
}

template <class T>
HLinearMixer<T>::HLinearMixer(HReader<T>* reader, HWriterConsumer<T>* consumer, size_t blocksize):
    _reader_1(reader),
    _reader_2(NULL),
    _writer(NULL),
    _blocksize(blocksize)
{
    HLog("Hlinearmixer(HReader*, HWriterConsumer*)");
    Init(blocksize);
}

template <class T>
HLinearMixer<T>::~HLinearMixer()
{
    HLog("~HLinearMixer()");
    delete _buffer_1;
    delete _buffer_2;
}

template <class T>
void HLinearMixer<T>::Init(size_t blocksize)
{
    _buffer_1 = new T[blocksize];
    _buffer_2 = new T[blocksize];
    HLog("Allocated 2 X %d as local buffers", blocksize * sizeof(T));
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
        HLog("Zero read from reader-1, stopping %d", received_1);
        return 0;
    }
    if( received_2 <= 0)
    {
        HLog("Zero read from reader-2, stopping %d", received_2);
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

template <class T>
int HLinearMixer<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HlinearMixer. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to write more data than the size given at creation of the mixer");
    }

    // Read from the reader and mix with the input
    int received = _reader_1->Read(_buffer_1, blocksize);
    if( received < blocksize)
    {
        HLog("Short read from reader, stopping %d", received);
        return 0;
    }

    // Mix inputs
    for( int i = 0; i < blocksize; i++ )
    {
        _buffer_2[i] = _buffer_1[i] + src[i];
    }

    // Write to next writer
    int written = _writer->Write(_buffer_2, blocksize);
    if( written != blocksize )
    {
        HError("Short write to writer, stopping %d", written);
        return 0;
    }
    return blocksize;
}

template <class T>
bool HLinearMixer<T>::Start()
{
    HLog("Calling Start() on reader 1");
    if( !_reader_1->Start() )
    {
        return false;
    }
    HLog("Calling Start() on reader 2");
    return _reader_2->Start();
}

template <class T>
bool HLinearMixer<T>::Stop()
{
    HLog("Calling Stop() on reader 1");
    if( !_reader_1->Stop() )
    {
        return false;
    }
    HLog("Calling Stop() on reader 2");
    return _reader_2->Stop();
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HLinearMixer
template
HLinearMixer<int8_t>::HLinearMixer(HReader<int8_t>* reader_1, HReader<int8_t>* reader_2, size_t blocksize);

template
HLinearMixer<uint8_t>::HLinearMixer(HReader<uint8_t>* reader_1, HReader<uint8_t>* reader_2, size_t blocksize);

template
HLinearMixer<int16_t>::HLinearMixer(HReader<int16_t>* reader_1, HReader<int16_t>* reader_2, size_t blocksize);

template
HLinearMixer<int32_t>::HLinearMixer(HReader<int32_t>* reader_1, HReader<int32_t>* reader_2, size_t blocksize);

template
HLinearMixer<int8_t>::HLinearMixer(HReader<int8_t>* reader, HWriter<int8_t>* writer, size_t blocksize);

template
HLinearMixer<uint8_t>::HLinearMixer(HReader<uint8_t>* reader, HWriter<uint8_t>* writer, size_t blocksize);

template
HLinearMixer<int16_t>::HLinearMixer(HReader<int16_t>* reader, HWriter<int16_t>* writer, size_t blocksize);

template
HLinearMixer<int32_t>::HLinearMixer(HReader<int32_t>* reader, HWriter<int32_t>* writer, size_t blocksize);

template
HLinearMixer<int8_t>::HLinearMixer(HReader<int8_t>* reader, HWriterConsumer<int8_t>* consumer, size_t blocksize);

template
HLinearMixer<uint8_t>::HLinearMixer(HReader<uint8_t>* reader, HWriterConsumer<uint8_t>* consumer, size_t blocksize);

template
HLinearMixer<int16_t>::HLinearMixer(HReader<int16_t>* reader, HWriterConsumer<int16_t>* consumer, size_t blocksize);

template
HLinearMixer<int32_t>::HLinearMixer(HReader<int32_t>* reader, HWriterConsumer<int32_t>* consumer, size_t blocksize);

// ~HLinearMixer()
template
HLinearMixer<int8_t>::~HLinearMixer();

template
HLinearMixer<uint8_t>::~HLinearMixer();

template
HLinearMixer<int16_t>::~HLinearMixer();

template
HLinearMixer<int32_t>::~HLinearMixer();

// Init()
template
void HLinearMixer<int8_t>::Init(size_t blocksize);

template
void HLinearMixer<uint8_t>::Init(size_t blocksize);

template
void HLinearMixer<int16_t>::Init(size_t blocksize);

template
void HLinearMixer<int32_t>::Init(size_t blocksize);

// Read()
template
int HLinearMixer<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HLinearMixer<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HLinearMixer<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HLinearMixer<int32_t>::Read(int32_t* dest, size_t blocksize);

// Write()
template
int HLinearMixer<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HLinearMixer<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HLinearMixer<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HLinearMixer<int32_t>::Write(int32_t* src, size_t blocksize);

// Start()
template
bool HLinearMixer<int8_t>::Start();

template
bool HLinearMixer<uint8_t>::Start();

template
bool HLinearMixer<int16_t>::Start();

template
bool HLinearMixer<int32_t>::Start();

// Stop()
template
bool HLinearMixer<int8_t>::Stop();

template
bool HLinearMixer<uint8_t>::Stop();

template
bool HLinearMixer<int16_t>::Stop();

template
bool HLinearMixer<int32_t>::Stop();

//! @endcond
#endif