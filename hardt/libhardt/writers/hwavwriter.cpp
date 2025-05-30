#ifndef __HWAVWRITER_CPP
#define __HWAVWRITER_CPP

#include "hwavwriter.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
bool HWavWriter<T>::Start()
{
    // Call parent Start()
    HLog("Starting underlying HFileWriter");
    if( !HFileWriter<T>::Start() )
    {
        HError("Failed to start underlying HFileWriter");
        return false;
    }

    // We know that the header is _always_ 44 bytes long for any wav
    // file, regardless of format, rate and number of channels. So we
    // can safely divide 44 by the size of the datatype (1, 2) to get
    // the correct number of bytes to write
    HLog("Writing header");
    return HFileWriter<T>::WriteImpl((T*) &_header, 44 / sizeof(T)) == 44 / sizeof(T);
}

template <class T>
bool HWavWriter<T>::Stop()
{
    // Calculate size fields
    HLog("Calculate final size, %d blocks has been written", _size);
    uint32_t finalSize = _size * sizeof(T);
    _header.SubChunk_2_Size = (uint32_t) finalSize;
    _header.ChunkSize += _header.SubChunk_2_Size;
    HLog("Final size set to %d", finalSize);

    // Update header
    HFileWriter<T>::Seek(0);
    HLog("Updating header");
    HFileWriter<T>::WriteImpl((T*) &_header, 44 / sizeof(T));

    // Call parent Stop()
    HLog("Stopping underlying HFileWriter");
    return HFileWriter<T>::Stop();
}

template <class T>
int HWavWriter<T>::WriteImpl(T* dest, size_t blocksize)
{
    // Do note that size contains the size in blocks, eg. number of samples,
    // not the final size in bytes..!
    _size += blocksize;
    return HFileWriter<T>::WriteImpl(dest, blocksize);
}


/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// Start()
template
bool HWavWriter<int8_t>::Start();

template
bool HWavWriter<uint8_t>::Start();

template
bool HWavWriter<int16_t>::Start();

template
bool HWavWriter<int32_t>::Start();

// Stop()
template
bool HWavWriter<int8_t>::Stop();

template
bool HWavWriter<uint8_t>::Stop();

template
bool HWavWriter<int16_t>::Stop();

template
bool HWavWriter<int32_t>::Stop();

// Write()
template
int HWavWriter<int8_t>::WriteImpl(int8_t* dest, size_t blocksize);

template
int HWavWriter<uint8_t>::WriteImpl(uint8_t* dest, size_t blocksize);

template
int HWavWriter<int16_t>::WriteImpl(int16_t* dest, size_t blocksize);

template
int HWavWriter<int32_t>::WriteImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif