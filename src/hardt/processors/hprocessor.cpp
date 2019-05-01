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
HProcessor<int>::HProcessor(HWriter<int>* writer, HReader<int>* reader);

// Read()
template
int HProcessor<int>::Read(int* dest, int blocksize);

// Write()
template
int HProcessor<int>::Write(int* src, int blocksize);

// Start()
template
bool HProcessor<int>::Start(void* data);

// Stop()
template
bool HProcessor<int>::Stop();

#endif