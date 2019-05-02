#ifndef __HPROCESSOR_CPP
#define __HPROCESSOR_CPP

#include "hprocessor.h"

template <class T>
HProcessor<T>::HProcessor(HWriter<T>* writer, HReader<T>* reader):
    _writer(writer),
    _reader(reader)
{}

template <class T>
int HProcessor<T>::Read(T* dest, int blocksize)
{
    try
    {
        return _reader->Read(dest, blocksize);
    }
    catch(std::exception e)
    {
        HError("Exception in Write(): %s", e.what());
        return 0;
    }
}

template <class T>
int HProcessor<T>::Write(T* src, int blocksize)
{
    try
    {
        return _writer->Write(src, blocksize);
    }
    catch(std::exception e)
    {
        HError("Exception in Read(): %s", e.what());
        return 0;
    }
}

template <class T>
bool HProcessor<T>::Start(void* data)
{
    return _writer->Start(data) && _reader->Start(data);
}

template <class T>
bool HProcessor<T>::Stop()
{
    return _writer->Stop() && _reader->Stop();
}

template <class T>
void HProcessor<T>::SetReader(HReader<T>* reader)
{
    _reader = reader;
}

template <class T>
void HProcessor<T>::SetWriter(HWriter<T>* writer)
{
    _writer = writer;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HProcessor()
template
HProcessor<int8_t>::HProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader);

template
HProcessor<uint8_t>::HProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader);

template
HProcessor<int16_t>::HProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader);

template
HProcessor<int32_t>::HProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader);

// Read()
template
int HProcessor<int8_t>::Read(int8_t* dest, int blocksize);

template
int HProcessor<uint8_t>::Read(uint8_t* dest, int blocksize);

template
int HProcessor<int16_t>::Read(int16_t* dest, int blocksize);

template
int HProcessor<int32_t>::Read(int32_t* dest, int blocksize);

// Write()
template
int HProcessor<int8_t>::Write(int8_t* src, int blocksize);

template
int HProcessor<uint8_t>::Write(uint8_t* src, int blocksize);

template
int HProcessor<int16_t>::Write(int16_t* src, int blocksize);

template
int HProcessor<int32_t>::Write(int32_t* src, int blocksize);

// Start()
template
bool HProcessor<int8_t>::Start(void* data);

template
bool HProcessor<uint8_t>::Start(void* data);

template
bool HProcessor<int16_t>::Start(void* data);

template
bool HProcessor<int32_t>::Start(void* data);

// Stop()
template
bool HProcessor<int8_t>::Stop();

template
bool HProcessor<uint8_t>::Stop();

template
bool HProcessor<int16_t>::Stop();

template
bool HProcessor<int32_t>::Stop();

#endif