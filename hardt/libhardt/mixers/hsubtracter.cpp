#ifndef __HSUBTRACTER_CPP
#define __HSUBTRACTER_CPP

#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hprobe.h"

#include "hsubtracter.h"

template <class T>
HSubtracter<T>::HSubtracter(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize, HProbe<T>* probe):
    _reader_1(reader_1),
    _reader_2(reader_2),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe),
    _wait(false),
    _started(false)
{
    HLog("HSubtracter(HReader*, HReader*)");
    Init(blocksize);
}

template <class T>
HSubtracter<T>::HSubtracter(HReader<T>* reader, HWriter<T>* writer, size_t blocksize, HProbe<T>* probe):
    _reader_1(reader),
    _reader_2(NULL),
    _writer(writer),
    _blocksize(blocksize),
    _probe(probe),
    _wait(false),
    _started(false)
{
    HLog("HSubtracter(HReader*, HWriter*)");
    Init(blocksize);
}

template <class T>
HSubtracter<T>::HSubtracter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe):
    _reader_1(NULL),
    _reader_2(NULL),
    _writer(writer),
    _blocksize(blocksize),
    _probe(probe),
    _wait(false),
    _started(false)
{
    HLog("HSubtracter(HWriter*)");
    Init(blocksize);
}

template <class T>
HSubtracter<T>::HSubtracter(HReader<T>* reader, HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe):
    _reader_1(reader),
    _reader_2(NULL),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe),_wait(false),
    _started(false)
{
    HLog("HSubtracter(HReader*, HWriterConsumer*)");
    Init(blocksize);
    consumer->SetWriter(this->Writer());
}

template <class T>
HSubtracter<T>::HSubtracter(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe):
    _reader_1(NULL),
    _reader_2(NULL),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe),_wait(false),
    _started(false)
{
    HLog("HSubtracter(HWriterConsumer*)");
    Init(blocksize);
    consumer->SetWriter(this->Writer());
}

template <class T>
HSubtracter<T>::~HSubtracter()
{
    HLog("~HSubtracter()");
    delete _buffer_1;
    delete _buffer_2;
}

template <class T>
void HSubtracter<T>::Init(size_t blocksize)
{
    _buffer_1 = new T[blocksize];
    _buffer_2 = new T[blocksize];
    HLog("Allocated 2 X %d as local buffers", blocksize * sizeof(T));
}

template <class T>
int HSubtracter<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HSubtracter. Initialized with %d called with %d", _blocksize, blocksize);
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

    // Subtract inputs
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = _buffer_1[i] - _buffer_2[i];
    }

    ((HReader<T>*) this)->Metrics.Reads++;
    ((HReader<T>*) this)->Metrics.BlocksIn++;
    ((HReader<T>*) this)->Metrics.BytesIn += blocksize * sizeof(T);
    if( _probe != NULL )
    {
        _probe->Write(dest, blocksize);
    }

    return blocksize;
}

template <class T>
int HSubtracter<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HSubtracter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to write more data than the size given at creation of the mixer");
    }

    // Handle multiplexed input from alternating writers
    if( _reader_1 == nullptr && _reader_2 == nullptr && _wait == false ) {

        // Copy first write to buffer-1 then wait for next write
        memcpy((void*) _buffer_1, src, sizeof(T) * blocksize);
        _wait = true;
        ((HWriter<T>*) this)->Metrics.Writes++;
        return blocksize;
    } else if( _reader_1 == nullptr && _reader_2 == nullptr && _wait == true ) {

        // Second write
        _wait = false;
    } else {

        // Read from the reader and subtract the input
        int received = _reader_1->Read(_buffer_1, blocksize);
        if( received < blocksize)
        {
            HLog("Short read from reader, stopping %d", received);
            return 0;
        }
    }

    // Subtract inputs
    for( int i = 0; i < blocksize; i++ )
    {
        _buffer_2[i] = _buffer_1[i] - src[i];
    }

    // Write to next writer
    int written = _writer->Write(_buffer_2, blocksize);
    if( written != blocksize )
    {
        HError("Short write to writer, stopping %d", written);
        return 0;
    }

    ((HWriter<T>*) this)->Metrics.Writes++;
    ((HWriter<T>*) this)->Metrics.BlocksOut++;
    ((HWriter<T>*) this)->Metrics.BytesOut += blocksize * sizeof(T);
    if( _probe != NULL )
    {
        _probe->Write(_buffer_2, blocksize);
    }

    return blocksize;
}

template <class T>
bool HSubtracter<T>::Start()
{
    if( _started == true ) {
        HLog("Discarding Start() to already started subtracter");
        return true;
    }
    HLog("Propagating Start()");
    _started = true;
    if( _reader_1 != NULL ) {
        HLog("Calling Start() on reader 1");
        if( !_reader_1->Start() )
        {
            return false;
        }
    }
    if( _reader_2 != NULL ) {
        HLog("Calling Start() on reader 2");
        return _reader_2->Start();
    }
    if( _writer != NULL ) {
        HLog("Calling Start() on writer");
        return _writer->Start();
    }
    return false;
}

template <class T>
bool HSubtracter<T>::Stop()
{
    HLog("Propagating Stop()");
    if( !_started ) {
        HLog("Discarding Stop() to already stopped subtracter");
        return true;
    }
    _started = false;
    if( _reader_1 != NULL ) {
        HLog("Calling Stop() on reader 1");
        if( !_reader_1->Stop() )
        {
            return false;
        }
    }
    if( _reader_2 != NULL ) {
        HLog("Calling Stop() on reader 2");
        return _reader_2->Stop();
    }
    if( _writer != NULL ) {
        HLog("Calling Stop() on writer");
        return _writer->Stop();
    }
    return false;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HSubtracter
template
HSubtracter<int8_t>::HSubtracter(HReader<int8_t>* reader_1, HReader<int8_t>* reader_2, size_t blocksize, HProbe<int8_t>* probe);

template
HSubtracter<uint8_t>::HSubtracter(HReader<uint8_t>* reader_1, HReader<uint8_t>* reader_2, size_t blocksize, HProbe<uint8_t>* probe);

template
HSubtracter<int16_t>::HSubtracter(HReader<int16_t>* reader_1, HReader<int16_t>* reader_2, size_t blocksize, HProbe<int16_t>* probe);

template
HSubtracter<int32_t>::HSubtracter(HReader<int32_t>* reader_1, HReader<int32_t>* reader_2, size_t blocksize, HProbe<int32_t>* probe);

template
HSubtracter<int8_t>::HSubtracter(HReader<int8_t>* reader, HWriter<int8_t>* writer, size_t blocksize, HProbe<int8_t>* probe);

template
HSubtracter<uint8_t>::HSubtracter(HReader<uint8_t>* reader, HWriter<uint8_t>* writer, size_t blocksize, HProbe<uint8_t>* probe);

template
HSubtracter<int16_t>::HSubtracter(HReader<int16_t>* reader, HWriter<int16_t>* writer, size_t blocksize, HProbe<int16_t>* probe);

template
HSubtracter<int32_t>::HSubtracter(HReader<int32_t>* reader, HWriter<int32_t>* writer, size_t blocksize, HProbe<int32_t>* probe);

template
HSubtracter<int8_t>::HSubtracter(HWriter<int8_t>* writer, size_t blocksize, HProbe<int8_t>* probe);

template
HSubtracter<uint8_t>::HSubtracter(HWriter<uint8_t>* writer, size_t blocksize, HProbe<uint8_t>* probe);

template
HSubtracter<int16_t>::HSubtracter(HWriter<int16_t>* writer, size_t blocksize, HProbe<int16_t>* probe);

template
HSubtracter<int32_t>::HSubtracter(HWriter<int32_t>* writer, size_t blocksize, HProbe<int32_t>* probe);

template
HSubtracter<int8_t>::HSubtracter(HReader<int8_t>* reader, HWriterConsumer<int8_t>* consumer, size_t blocksize, HProbe<int8_t>* probe);

template
HSubtracter<uint8_t>::HSubtracter(HReader<uint8_t>* reader, HWriterConsumer<uint8_t>* consumer, size_t blocksize, HProbe<uint8_t>* probe);

template
HSubtracter<int16_t>::HSubtracter(HReader<int16_t>* reader, HWriterConsumer<int16_t>* consumer, size_t blocksize, HProbe<int16_t>* probe);

template
HSubtracter<int32_t>::HSubtracter(HReader<int32_t>* reader, HWriterConsumer<int32_t>* consumer, size_t blocksize, HProbe<int32_t>* probe);

template
HSubtracter<int8_t>::HSubtracter(HWriterConsumer<int8_t>* consumer, size_t blocksize, HProbe<int8_t>* probe);

template
HSubtracter<uint8_t>::HSubtracter(HWriterConsumer<uint8_t>* consumer, size_t blocksize, HProbe<uint8_t>* probe);

template
HSubtracter<int16_t>::HSubtracter(HWriterConsumer<int16_t>* consumer, size_t blocksize, HProbe<int16_t>* probe);

template
HSubtracter<int32_t>::HSubtracter(HWriterConsumer<int32_t>* consumer, size_t blocksize, HProbe<int32_t>* probe);

// ~HSubtracter()
template
HSubtracter<int8_t>::~HSubtracter();

template
HSubtracter<uint8_t>::~HSubtracter();

template
HSubtracter<int16_t>::~HSubtracter();

template
HSubtracter<int32_t>::~HSubtracter();

// Init()
template
void HSubtracter<int8_t>::Init(size_t blocksize);

template
void HSubtracter<uint8_t>::Init(size_t blocksize);

template
void HSubtracter<int16_t>::Init(size_t blocksize);

template
void HSubtracter<int32_t>::Init(size_t blocksize);

// Read()
template
int HSubtracter<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HSubtracter<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HSubtracter<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HSubtracter<int32_t>::Read(int32_t* dest, size_t blocksize);

// Write()
template
int HSubtracter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HSubtracter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HSubtracter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HSubtracter<int32_t>::Write(int32_t* src, size_t blocksize);

// Start()
template
bool HSubtracter<int8_t>::Start();

template
bool HSubtracter<uint8_t>::Start();

template
bool HSubtracter<int16_t>::Start();

template
bool HSubtracter<int32_t>::Start();

// Stop()
template
bool HSubtracter<int8_t>::Stop();

template
bool HSubtracter<uint8_t>::Stop();

template
bool HSubtracter<int16_t>::Stop();

template
bool HSubtracter<int32_t>::Stop();

//! @endcond
#endif