#ifndef __HCOLLECTOR_CPP
#define __HCOLLECTOR_CPP

#include "hcollector.h"

template <class T>
HCollector<T>::HCollector(HWriter<T>* writer, size_t blocksizeIn, size_t blocksizeOut):
        _blocksizeIn(blocksizeIn),
        _blocksizeOut(blocksizeOut),
        _writer(writer),
        _reader(nullptr),
        _collected(0)
{
    HLog("HCollector(HWriter*, blocksizeIn=%d, blocksizeOut=%d)", blocksizeIn, blocksizeOut);
    _buffer = new T[_blocksizeOut];
}

template <class T>
HCollector<T>::HCollector(HWriterConsumer<T>* consumer, size_t blocksizeIn, size_t blocksizeOut):
        _blocksizeIn(blocksizeIn),
        _blocksizeOut(blocksizeOut),
        _writer(nullptr),
        _reader(nullptr),
        _collected(0)
{
    HLog("HCollector(HWriterConsumer*, blocksizeIn=%d, blocksizeOut=%d)", blocksizeIn, blocksizeOut);
    _buffer = new T[_blocksizeOut];
    consumer->SetWriter(this);

}

template <class T>
HCollector<T>::HCollector(HReader<T>* reader, size_t blocksizeIn, size_t blocksizeOut):
        _blocksizeIn(blocksizeIn),
        _blocksizeOut(blocksizeOut),
        _writer(nullptr),
        _reader(reader),
        _collected(0)
{
    HLog("HCollector(HReader*, blocksizeIn=%d, blocksizeOut=%d)", blocksizeIn, blocksizeOut);
    _buffer = nullptr;
}

template <class T>
HCollector<T>::~HCollector()
{
    if( _buffer != nullptr ) {
        delete[] _buffer;
    }
}

template <class T>
int HCollector<T>::Write(T* src, size_t blocksize)
{
    if( blocksize != _blocksizeIn )
    {
        HError("Requested blocksize for write is incorrect: %d requested, expected is %d", blocksize, _blocksizeIn);
        throw new HWriterIOException("Requested blocksize for write is incorrect");
    }

    memcpy((void*) &_buffer[_collected], (void*) src, _blocksizeIn * sizeof(T));
    _collected += blocksize;

    if( _collected == _blocksizeOut ) {
        _collected = 0;
        if( _writer->Write(_buffer, _blocksizeOut) == 0 ) {
            HLog("Zerolength write to downstream writer, stopping");
            return 0;
        }
    }
    return blocksize;
}

template <class T>
int HCollector<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize != _blocksizeOut )
    {
        HError("Requested blocksize for read is incorrect: %d requested, expected is %d", blocksize, _blocksizeOut);
        throw new HReaderIOException("Requested blocksize for read is too big");
    }

    for( _collected = 0; _collected < _blocksizeOut; _collected += _blocksizeIn ) {
        if( _reader->Read(&dest[_collected], _blocksizeIn) == 0 ) {
            HLog("zerolength read from upstream reader, stopping");
            return 0;
        }
    }
    return blocksize;
}

template <class T>
void HCollector<T>::SetWriter(HWriter<T>* writer)
{
    _writer = writer;
}

template <class T>
bool HCollector<T>::Start()
{
    if( _writer != nullptr )
    {
        return _writer->Start();
    }
    if( _reader != nullptr )
    {
        return _reader->Start();
    }
    return true;
}

template <class T>
bool HCollector<T>::Stop()
{
    if( _writer != nullptr )
    {
        return _writer->Stop();
    }
    if( _reader != nullptr )
    {
        return _reader->Stop();
    }
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HCollector()
template
HCollector<int8_t>::HCollector(HWriter<int8_t>* writer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<uint8_t>::HCollector(HWriter<uint8_t>* writer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int16_t>::HCollector(HWriter<int16_t>* writer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int32_t>::HCollector(HWriter<int32_t>* writer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int8_t>::HCollector(HWriterConsumer<int8_t>* consumer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<uint8_t>::HCollector(HWriterConsumer<uint8_t>* consumer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int16_t>::HCollector(HWriterConsumer<int16_t>* consumer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int32_t>::HCollector(HWriterConsumer<int32_t>* consumer, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int8_t>::HCollector(HReader<int8_t>* reader, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<uint8_t>::HCollector(HReader<uint8_t>* reader, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int16_t>::HCollector(HReader<int16_t>* reader, size_t blocksizeIn, size_t blocksizeOut);

template
HCollector<int32_t>::HCollector(HReader<int32_t>* reader, size_t blocksizeIn, size_t blocksizeOut);

// ~HCollector()
template
HCollector<int8_t>::~HCollector();

template
HCollector<uint8_t>::~HCollector();

template
HCollector<int16_t>::~HCollector();

template
HCollector<int32_t>::~HCollector();

// Write()
template
int HCollector<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HCollector<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HCollector<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HCollector<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HCollector<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HCollector<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HCollector<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HCollector<int32_t>::Read(int32_t* dest, size_t blocksize);

// SetWriter()
template
void HCollector<int8_t>::SetWriter(HWriter<int8_t>* writer);

template
void HCollector<uint8_t>::SetWriter(HWriter<uint8_t>* writer);

template
void HCollector<int16_t>::SetWriter(HWriter<int16_t>* writer);

template
void HCollector<int32_t>::SetWriter(HWriter<int32_t>* writer);

// Start()
template
bool HCollector<int8_t>::Start();

template
bool HCollector<uint8_t>::Start();

template
bool HCollector<int16_t>::Start();

template
bool HCollector<int32_t>::Start();

// Stop()
template
bool HCollector<int8_t>::Stop();

template
bool HCollector<uint8_t>::Stop();

template
bool HCollector<int16_t>::Stop();

template
bool HCollector<int32_t>::Stop();

//! @endcond
#endif
