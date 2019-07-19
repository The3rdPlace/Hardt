#ifndef __HCASCADEDBIQUADFACTORY_CPP
#define __HCASCADEDBIQUADFACTORY_CPP

#include "hcascadedbiquadfilter.h"

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize):
    _writer(writer),
    _reader(NULL),
    _blocksize(blocksize)
{
    HLog("HCascadedBiQuadFilter(HWriter*)");
    if( length % 5 != 0 )
    {
        HError("A cascaded biquad filter must consist of N sets of 5 coefficients");
        throw new HFilterInitializationException("Invalid number of coefficients in creation of a cascaded biquad filter");
    }
    Init(coefficients, length);
}

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize):
    _writer(NULL),
    _reader(reader),
    _blocksize(blocksize)
{
    HLog("HCascadedBiQuadFilter(HReader*)");
    if( length % 5 != 0 )
    {
        HError("A cascaded biquad filter must consist of N sets of 5 coefficients");
        throw new HFilterInitializationException("Invalid number of coefficients in creation of a cascaded biquad filter");
    }
    Init(coefficients, length);
}

template <class T>
void HCascadedBiQuadFilter<T>::Init(float* coefficients, int length)
{
    // Number of filters
    _filterCount = length / 5;

    // Allocate filter-list
    _filters = new HIirFilter<T>*[_filterCount];
    HLog("Allocated list for %d filters", _filterCount);

    // Create filters
    if( _reader != NULL )
    {
        for( int i = 0; i < _filterCount; i++ )
        {
            if( i == 0 )
            {
                _filters[i] = new HIirFilter<T>((HReader<T>*) _reader, &coefficients[i * 5], 5, 4096);
            }
            else
            {
                _filters[i] = new HIirFilter<T>((HReader<T>*) _filters[i - 1], &coefficients[i * 5], 5, 4096);
            }
        }
    }
    else if ( _writer != NULL )
    {
        for( int i = _filterCount -1; i >= 0; i-- )
        {
            if( i == _filterCount - 1 )
            {
                _filters[i] = new HIirFilter<T>((HWriter<T>*) _writer, &coefficients[i * 5], 5, 4096);
            }
            else
            {
                _filters[i] = new HIirFilter<T>((HWriter<T>*) _filters[i + 1], &coefficients[i * 5], 5, 4096);
            }
        }
    }
    else
    {
         HError("No reader or writer given");
         throw new HFilterInitializationException("No reader or writer given when creating a cascaded biquad filter");
    }
}

template <class T>
HCascadedBiQuadFilter<T>::~HCascadedBiQuadFilter()
{
    HLog("~HCascadedBiQuadFilter()");
    for( int i = 0; i < _filterCount; i++ )
    {
        delete _filters[i];
    }
    delete[] _filters;
}

template <class T>
int HCascadedBiQuadFilter<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HCascadedBiQuadFilter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not allowed to write more data than the size given at creation of the filter");
    }

    return _filters[0]->Write(src, blocksize);
}

template <class T>
int HCascadedBiQuadFilter<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HCascadedBiQuadFilter. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation of the filter");
    }

    return _filters[_filterCount - 1]->Read(dest, blocksize);
}

template <class T>
bool HCascadedBiQuadFilter<T>::Start()
{
    if( _reader != NULL )
    {
        HLog("Calling Start() on last filter");
        return _filters[_filterCount - 1]->Start();
    }
    if( _writer != NULL )
    {
        HLog("Calling Start() on first filter");
        return _filters[0]->Start();
    }
    return false;
}

template <class T>
bool HCascadedBiQuadFilter<T>::Stop()
{
    if( _reader != NULL )
    {
        HLog("Calling Stop() on last filter");
        return _filters[_filterCount - 1]->Stop();
    }
    if( _writer != NULL )
    {
        HLog("Calling Stop() on first filter");
        return _filters[0]->Stop();
    }
    return false;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HCascadedBiQuadFilter
template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize);

// ~HCascadedBiQuadFilter()
template
HCascadedBiQuadFilter<int8_t>::~HCascadedBiQuadFilter();

template
HCascadedBiQuadFilter<uint8_t>::~HCascadedBiQuadFilter();

template
HCascadedBiQuadFilter<int16_t>::~HCascadedBiQuadFilter();

template
HCascadedBiQuadFilter<int32_t>::~HCascadedBiQuadFilter();

// Write()
template
int HCascadedBiQuadFilter<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HCascadedBiQuadFilter<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HCascadedBiQuadFilter<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HCascadedBiQuadFilter<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HCascadedBiQuadFilter<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HCascadedBiQuadFilter<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HCascadedBiQuadFilter<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HCascadedBiQuadFilter<int32_t>::Read(int32_t* dest, size_t blocksize);

// Start()
template
bool HCascadedBiQuadFilter<int8_t>::Start();

template
bool HCascadedBiQuadFilter<uint8_t>::Start();

template
bool HCascadedBiQuadFilter<int16_t>::Start();

template
bool HCascadedBiQuadFilter<int32_t>::Start();

// Stop()
template
bool HCascadedBiQuadFilter<int8_t>::Stop();

template
bool HCascadedBiQuadFilter<uint8_t>::Stop();

template
bool HCascadedBiQuadFilter<int16_t>::Stop();

template
bool HCascadedBiQuadFilter<int32_t>::Stop();

#endif