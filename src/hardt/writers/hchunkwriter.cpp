#ifndef __HCHUNKWRITER_CPP
#define __HCHUNKWRITER_CPP

#include "hchunkwriter.h"

template <class T>
int HChunkWriter<T>::Write(T* src, size_t blocksize)
{
    // Check if chunking is disabled
    if( _chunksize == 0 )
    {
        return WriteChunk(src, blocksize);
    }

    // Check if we can split the input into an integer number of chunks
    if( blocksize % _chunksize != 0 )
    {
        HError("Request for write with blocksize %d, with chunksize %d = %f chunks", blocksize, _chunksize, blocksize / _chunksize);
        throw new HWriterIOException("Request for write with non-integer chunksize");
    }

    // Write chunks
    for( int i = 0; i < blocksize / _chunksize; i++ )
    {
        WriteChunk(&src[i * _chunksize], _chunksize);
    }

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// WriteChunk()
template
int HChunkWriter<int8_t>::Write(int8_t* src, size_t chunkSize);

template
int HChunkWriter<uint8_t>::Write(uint8_t* src, size_t chunkSize);

template
int HChunkWriter<int16_t>::Write(int16_t* src, size_t chunkSize);

template
int HChunkWriter<int32_t>::Write(int32_t* src, size_t chunkSize);

#endif