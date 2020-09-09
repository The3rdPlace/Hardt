#ifndef __HMUX_CPP
#define __HMUX_CPP

#include "hmux.h"

template <class T>
HMux<T>::HMux( std::vector< HReader<T>* > readers, size_t blocksize):
    _readers(readers)
{
    HLog("HMux(%d readers)", readers.size());

    _buffers = new T*[readers.size()];
    for( int i = 0; i < readers.size(); i++ )
    {
        _buffers[i] = new T[blocksize / readers.size()];
    }
}

template <class T>
HMux<T>::~HMux()
{
    HLog("~HMux()");
    for(int i = 0; i < _readers.size(); i++ )
    {
        delete _buffers[i];
    }
    delete _buffers;
}

template <class T>
int HMux<T>::Read(T* dest, size_t blocksize)
{
    for( int i = 0; i < _readers.size(); i++ )
    {
        int read = _readers[i]->Read(_buffers[i], blocksize / _readers.size() );
        if( read != (blocksize / _readers.size() ) )
        {
            HError("Incorrect read with size %d for reader %d, returning zero", read, i);
            return 0;
        }
    }

    int pos = 0;
    int reader = 0;
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = _buffers[reader++][pos];
        if( reader >= _readers.size() )
        {
            reader = 0;
            pos++;
        }
    }

    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HMux
template
HMux<int8_t>::HMux( std::vector< HReader<int8_t>* > readers, size_t blocksize);

template
HMux<uint8_t>::HMux( std::vector< HReader<uint8_t>* > readers, size_t blocksize);

template
HMux<int16_t>::HMux( std::vector< HReader<int16_t>* > readers, size_t blocksize);

template
HMux<int32_t>::HMux( std::vector< HReader<int32_t>* > readers, size_t blocksize);

// ~HMux()
template
HMux<int8_t>::~HMux();

template
HMux<uint8_t>::~HMux();

template
HMux<int16_t>::~HMux();

template
HMux<int32_t>::~HMux();

// Write
template
int HMux<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HMux<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HMux<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HMux<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif