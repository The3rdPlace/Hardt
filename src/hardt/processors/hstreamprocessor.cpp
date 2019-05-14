#ifndef __HSTREAMPROCESSOR_CPP
#define __HSTREAMPROCESSOR_CPP

#include "hstreamprocessor.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HStreamProcessor<T>::HStreamProcessor(HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, reader),
    _terminated(terminationToken),
    _buffer(NULL),
    _blocksize(blocksize)
{
    HLog("HStreamProcessor(...), blocksize is %d", _blocksize);

    // Allocate local buffer
    _buffer = new T[_blocksize];
    HLog("Allocated buffer for %d frames = %d bytes", _blocksize, _blocksize * sizeof(T));
}

template <class T>
HStreamProcessor<T>::~HStreamProcessor()
{
    HLog("~HStreamProcessor()");
    if( _buffer != NULL )
    {
        HLog("Releasing local buffer");
        delete _buffer;
    }
    HLog("Done");
}

template <class T>
void HStreamProcessor<T>::Run()
{
    // Start reader and writer - some readers/writers have start/stop handling
    HLog("Starting reader and writer, data is NULL");
    if( !HProcessor<T>::Start(NULL) )
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

    // Dump in- and output metrics to the log
    HLog(this->GetMetrics("HStreamProcessor").c_str());
}

template <class T>
void HStreamProcessor<T>::Halt()
{
    HLog("Halting processor");
    *_terminated = true;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HStreamProcessor()
template
HStreamProcessor<int8_t>::HStreamProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<uint8_t>::HStreamProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int16_t>::HStreamProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int32_t>::HStreamProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

// ~HStreamProcessor
template
HStreamProcessor<int8_t>::~HStreamProcessor();

template
HStreamProcessor<uint8_t>::~HStreamProcessor();

template
HStreamProcessor<int16_t>::~HStreamProcessor();

template
HStreamProcessor<int32_t>::~HStreamProcessor();

// Run()
template
void HStreamProcessor<int8_t>::Run();

template
void HStreamProcessor<uint8_t>::Run();

template
void HStreamProcessor<int16_t>::Run();

template
void HStreamProcessor<int32_t>::Run();

// Halt()
template
void HStreamProcessor<int8_t>::Halt();

template
void HStreamProcessor<uint8_t>::Halt();

template
void HStreamProcessor<int16_t>::Halt();

template
void HStreamProcessor<int32_t>::Halt();

#endif