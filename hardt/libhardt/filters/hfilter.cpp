#ifndef __HFILTER_CPP
#define __HFILTER_CPP

#include "hwriter.h"
#include "hreader.h"
#include "hfilterbase.h"
#include "hfilter.h"
#include "hprobe.h"

template <class T>
HFilter<T>::HFilter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe):
    _writer(writer),
    _reader(NULL),
    _blocksize(blocksize),
    _probe(probe)
{
    HLog("HFilter(HWriter*)");
    Init();
}

template <class T>
HFilter<T>::HFilter(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe):
    _reader(NULL),
    _blocksize(blocksize),
    _probe(probe)
{
    HLog("HFilter(HWriter*)");
    Init();

    consumer->SetWriter(this);
}

template <class T>
HFilter<T>::HFilter(HReader<T>* reader, size_t blocksize, HProbe<T>* probe):
    _writer(NULL),
    _reader(reader),
    _blocksize(blocksize),
    _probe(probe)
{
    HLog("HFilter(HReader*)");
    Init();
}

template <class T>
HFilter<T>::~HFilter()
{
    HLog("~HFilter()");
    delete _buffer;
}

template <class T>
void HFilter<T>::Init()
{
    _buffer = new T[_blocksize];
    HLog("Allocated %d as local buffer", _blocksize * sizeof(T));
}

template <class T>
int HFilter<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HFilter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not allowed to write more data than the size given at creation of the filter");
    }

    Filter(src, _buffer, blocksize);
    int written = _writer->Write(_buffer, blocksize);

    ((HFilterBase<T>*) this)->Metrics.Writes++;
    ((HFilterBase<T>*) this)->Metrics.BytesOut += blocksize * sizeof(T);
    if( _probe != NULL )
    {
        _probe->Write(_buffer, blocksize);
    }

    return written;
}

template <class T>
int HFilter<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HFilter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation of the filter");
    }

    int received = _reader->Read(_buffer, blocksize);
    Filter(_buffer, dest, received);

    ((HFilterBase<T>*) this)->Metrics.Reads++;
    ((HFilterBase<T>*) this)->Metrics.BytesIn += blocksize * sizeof(T);
    if( _probe != NULL )
    {
        _probe->Write(_buffer, blocksize);
    }

    return received;
}

template <class T>
bool HFilter<T>::Start()
{
    if( _reader != NULL )
    {
        HLog("Calling Start() on reader");
        return _reader->Start();
    }
    if( _writer != NULL )
    {
        HLog("Calling Start() on writer");
        return _writer->Start();
    }
    return false;
}

template <class T>
bool HFilter<T>::Stop()
{
    if( _reader != NULL )
    {
        HLog("Calling Stop() on reader");
        return _reader->Stop();
    }
    if( _writer != NULL )
    {
        HLog("Calling Stop() on writer");
        return _writer->Stop();
    }
    return false;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HFilter
template
HFilter<int8_t>::HFilter(HWriter<int8_t>* writer, size_t blocksize, HProbe<int8_t>* probe);

template
HFilter<uint8_t>::HFilter(HWriter<uint8_t>* writer, size_t blocksize, HProbe<uint8_t>* probe);

template
HFilter<int16_t>::HFilter(HWriter<int16_t>* writer, size_t blocksize, HProbe<int16_t>* probe);

template
HFilter<int32_t>::HFilter(HWriter<int32_t>* writer, size_t blocksize, HProbe<int32_t>* probe);

template
HFilter<int8_t>::HFilter(HWriterConsumer<int8_t>* consumer, size_t blocksize, HProbe<int8_t>* probe);

template
HFilter<uint8_t>::HFilter(HWriterConsumer<uint8_t>* consumer, size_t blocksize, HProbe<uint8_t>* probe);

template
HFilter<int16_t>::HFilter(HWriterConsumer<int16_t>* consumer, size_t blocksize, HProbe<int16_t>* probe);

template
HFilter<int32_t>::HFilter(HWriterConsumer<int32_t>* consumer, size_t blocksize, HProbe<int32_t>* probe);

template
HFilter<int8_t>::HFilter(HReader<int8_t>* reader, size_t blocksize, HProbe<int8_t>* probe);

template
HFilter<uint8_t>::HFilter(HReader<uint8_t>* reader, size_t blocksize, HProbe<uint8_t>* probe);

template
HFilter<int16_t>::HFilter(HReader<int16_t>* reader, size_t blocksize, HProbe<int16_t>* probe);

template
HFilter<int32_t>::HFilter(HReader<int32_t>* reader, size_t blocksize, HProbe<int32_t>* probe);

// ~HFilter()
template
HFilter<int8_t>::~HFilter();

template
HFilter<uint8_t>::~HFilter();

template
HFilter<int16_t>::~HFilter();

template
HFilter<int32_t>::~HFilter();

// Write()
template
int HFilter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HFilter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HFilter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HFilter<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HFilter<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HFilter<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HFilter<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HFilter<int32_t>::Read(int32_t* dest, size_t blocksize);

// Start()
template
bool HFilter<int8_t>::Start();

template
bool HFilter<uint8_t>::Start();

template
bool HFilter<int16_t>::Start();

template
bool HFilter<int32_t>::Start();

// Stop()
template
bool HFilter<int8_t>::Stop();

template
bool HFilter<uint8_t>::Stop();

template
bool HFilter<int16_t>::Stop();

template
bool HFilter<int32_t>::Stop();

// Init()
template
void HFilter<int8_t>::Init();

template
void HFilter<uint8_t>::Init();

template
void HFilter<int16_t>::Init();

template
void HFilter<int32_t>::Init();

//! @endcond
#endif