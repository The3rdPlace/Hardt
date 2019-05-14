#ifndef __HPROCESSOR_CPP
#define __HPROCESSOR_CPP

#include "hprocessor.h"

template <class T>
HProcessor<T>::HProcessor(HWriter<T>* writer, HReader<T>* reader, size_t blocksize, bool* terminationToken):
    _writer(writer),
    _reader(reader),
    _blocksize(blocksize),
    _terminated(terminationToken)
{
    HLog("HProcessor(...), blocksize is %d", _blocksize);

    // Allocate local buffer
    _buffer = new T[_blocksize];
    HLog("Allocated buffer for %d frames = %d bytes", _blocksize, _blocksize * sizeof(T));
}

template <class T>
HProcessor<T>::~HProcessor()
{
    HLog("~HProcessor()");
    if( _buffer != NULL )
    {
        HLog("Releasing local buffer");
        delete _buffer;
    }
    HLog("Done");
}

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

template <class T>
void HProcessor<T>::Run(void* startData)
{
    // Start reader and writer - some readers/writers have start/stop handling
    HLog("Starting reader and writer");
    if( !HProcessor<T>::Start(startData) )
    {
        HError("Failed to Start() reader or writer");
        return;
    }
    HLog("Reader and writer Start()'ed");

    // Read from reader and write to network
    HLog("Processing");
    while(!*_terminated)
    {
        // Read data from the reader
        int len;
        try
        {
            len = HProcessor<T>::Read(_buffer, _blocksize);
            if( len == 0 )
            {
                HLog("Zero read from the reader, stopping");
                break;
            }
            this->Metrics.Reads++;
            this->Metrics.BlocksIn += len;
            this->Metrics.BytesIn += len * sizeof(T);
        }
        catch( std::exception ex )
        {
            HError("Caught exception: %s", ex.what());
            break;
        }

        // Write the received data
        int shipped;
        try
        {
            this->Metrics.Writes++;
            shipped = HProcessor<T>::Write(_buffer, len);
            if( shipped <= 0 )
            {
                HLog("Zero write to the writer, stopping");
                break;
            }
            if( shipped != len )
            {
                HLog("Not all data was written, %d of %d ", shipped, len);
            }
            this->Metrics.BlocksOut += shipped;
            this->Metrics.BytesOut += shipped * sizeof(T);
        }
        catch( std::exception ex )
        {
            HError("Caught exception: %s", ex.what());
            break;
        }
    }

    // Stop the reader - some readers have start/stop handling
    HLog("Stopping reader and writer");
    if( HProcessor<T>::Stop() == false )
    {
        HError("Failed to Stop() reader or writer");
    }
    HLog("Reader and writer Stop()'ed");
}

template <class T>
void HProcessor<T>::Halt()
{
    HLog("Halting processor");
    *_terminated = true;
}

template <class T>
HReader<T>* HProcessor<T>::GetReader()
{
    return _reader;
}

template <class T>
HWriter<T>* HProcessor<T>::GetWriter()
{
    return _writer;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HProcessor()
template
HProcessor<int8_t>::HProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader, size_t blocksize, bool* terminationToken);

template
HProcessor<uint8_t>::HProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader, size_t blocksize, bool* terminationToken);

template
HProcessor<int16_t>::HProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader, size_t blocksize, bool* terminationToken);

template
HProcessor<int32_t>::HProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader, size_t blocksize, bool* terminationToken);

// ~HProcessor
template
HProcessor<int8_t>::~HProcessor();

template
HProcessor<uint8_t>::~HProcessor();

template
HProcessor<int16_t>::~HProcessor();

template
HProcessor<int32_t>::~HProcessor();

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

// Run()
template
void HProcessor<int8_t>::Run(void* startData);

template
void HProcessor<uint8_t>::Run(void* startData);

template
void HProcessor<int16_t>::Run(void* startData);

template
void HProcessor<int32_t>::Run(void* startData);

// Halt()
template
void HProcessor<int8_t>::Halt();

template
void HProcessor<uint8_t>::Halt();

template
void HProcessor<int16_t>::Halt();

template
void HProcessor<int32_t>::Halt();

// GetReader()
template
HReader<int8_t>* HProcessor<int8_t>::GetReader();

template
HReader<uint8_t>* HProcessor<uint8_t>::GetReader();

template
HReader<int16_t>* HProcessor<int16_t>::GetReader();

template
HReader<int32_t>* HProcessor<int32_t>::GetReader();

// GetWriter()
template
HWriter<int8_t>* HProcessor<int8_t>::GetWriter();

template
HWriter<uint8_t>* HProcessor<uint8_t>::GetWriter();

template
HWriter<int16_t>* HProcessor<int16_t>::GetWriter();

template
HWriter<int32_t>* HProcessor<int32_t>::GetWriter();

#endif