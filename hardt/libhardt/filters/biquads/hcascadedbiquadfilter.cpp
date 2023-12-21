#ifndef __HCASCADEDBIQUADFACTORY_CPP
#define __HCASCADEDBIQUADFACTORY_CPP

#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"
#include "hreader.h"

#include <vector>

#include "hcascadedbiquadfilter.h"

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(std::string id, HWriter<T>* writer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isWriter(true),
    _isReader(false),
    _blocksize(blocksize),
    _firstLength(length)
{
    HLog("HCascadedBiQuadFilter(HWriter*)");
    if( length % 5 != 0 )
    {
        HError("A cascaded biquad filter must consist of N sets of 5 coefficients");
        throw new HFilterInitializationException("Invalid number of coefficients in creation of a cascaded biquad filter");
    }
    Init(length);

    // Create filters
    for( int i = _filterCount -1; i >= 0; i-- )
    {
        if( i == _filterCount - 1 )
        {
            _filters[i] = new HIirFilter<T>(id, writer, &coefficients[i * 5], 5, blocksize, probe);
        }
        else
        {
            _filters[i] = new HIirFilter<T>(id, _filters[i + 1]->Writer(), &coefficients[i * 5], 5, blocksize);
        }
    }
}

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isWriter(true),
    _isReader(false),
    _blocksize(blocksize),
    _firstLength(length)
{
    HLog("HCascadedBiQuadFilter(HWriterConsumer*)");
    if( length % 5 != 0 )
    {
        HError("A cascaded biquad filter must consist of N sets of 5 coefficients");
        throw new HFilterInitializationException("Invalid number of coefficients in creation of a cascaded biquad filter");
    }
    Init(length);

    // Create filters
    for( int i = 0; i < _filterCount; i++ )
    {
        if( i == 0 )
        {
            _filters[i] = new HIirFilter<T>(id, consumer, &coefficients[i * 5], 5, blocksize, probe);
        }
        else
        {
            _filters[i] = new HIirFilter<T>(id, _filters[i - 1]->Consumer(), &coefficients[i * 5], 5, blocksize);
        }
    }
}

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(std::string id, HReader<T>* reader, float* coefficients, int length, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isWriter(false),
    _isReader(true),
    _blocksize(blocksize),
    _firstLength(length)
{
    HLog("HCascadedBiQuadFilter(HReader*)");
    if( length % 5 != 0 )
    {
        HError("A cascaded biquad filter must consist of N sets of 5 coefficients");
        throw new HFilterInitializationException("Invalid number of coefficients in creation of a cascaded biquad filter");
    }
    Init(length);

    // Create filters
    for( int i = 0; i < _filterCount; i++ )
    {
        if( i == 0 )
        {
            _filters[i] = new HIirFilter<T>(id, reader, &coefficients[i * 5], 5, blocksize, probe);
        }
        else
        {
            _filters[i] = new HIirFilter<T>(id, _filters[i - 1]->Reader(), &coefficients[i * 5], 5, blocksize);
        }
    }
}

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(std::string id, HWriter<T>* writer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isWriter(true),
    _isReader(false),
    _blocksize(blocksize),
    _firstLength(biquadCoefficients.size() * 5)
{
    HLog("HCascadedBiQuadFilter(HWriter*)");
    Init(biquadCoefficients.size() * 5);

    // Create filters
    for( int i = _filterCount -1; i >= 0; i-- )
    {
        if( i == _filterCount - 1 )
        {
            _filters[i] = new HIirFilter<T>(id, writer, biquadCoefficients.at(i), 5, blocksize, probe);
        }
        else
        {
            _filters[i] = new HIirFilter<T>(id, _filters[i + 1]->Writer(), biquadCoefficients.at(i), 5, blocksize);
        }
    }
}

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<T>* consumer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isWriter(true),
    _isReader(false),
    _blocksize(blocksize),
    _firstLength(biquadCoefficients.size() * 5)
{
    HLog("HCascadedBiQuadFilter(HWriterConsumer*)");
    Init(biquadCoefficients.size() * 5);

    // Create filters
    for( int i = 0; i < _filterCount; i++ )
    {
        if( i == 0 )
        {
            _filters[i] = new HIirFilter<T>(id, consumer, biquadCoefficients.at(i), 5, blocksize, probe);
        }
        else
        {
            _filters[i] = new HIirFilter<T>(id, _filters[i - 1]->Consumer(), biquadCoefficients.at(i), 5, blocksize);
        }
    }
}

template <class T>
HCascadedBiQuadFilter<T>::HCascadedBiQuadFilter(std::string id, HReader<T>* reader, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isWriter(false),
    _isReader(true),
    _blocksize(blocksize),
    _firstLength(biquadCoefficients.size() * 5)
{
    HLog("HCascadedBiQuadFilter(HReader*)");
    Init(biquadCoefficients.size() * 5);

    // Create filters
    for( int i = 0; i < _filterCount; i++ )
    {
        if( i == 0 )
        {
            _filters[i] = new HIirFilter<T>(id, reader, biquadCoefficients.at(i), 5, blocksize, probe);
        }
        else
        {
            _filters[i] = new HIirFilter<T>(id, _filters[i - 1]->Reader(), biquadCoefficients.at(i), 5, blocksize);
        }
    }
}

template <class T>
void HCascadedBiQuadFilter<T>::Init(int length)
{
    // Number of filters
    _filterCount = length / 5;

    // Allocate filter-list
    _filters = new HIirFilter<T>*[_filterCount];
    HLog("Allocated list for %d filters", _filterCount);
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
int HCascadedBiQuadFilter<T>::ReadImpl(T* dest, size_t blocksize)
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
    if( _isReader )
    {
        HLog("Calling Start() on last filter");
        return _filters[_filterCount - 1]->Start();
    }
    if( _isWriter )
    {
        HLog("Calling Start() on first filter");
        return _filters[0]->Start();
    }
    return false;
}

template <class T>
bool HCascadedBiQuadFilter<T>::Stop()
{
    if( _isReader )
    {
        HLog("Calling Stop() on last filter");
        return _filters[_filterCount - 1]->Stop();
    }
    if( _isWriter )
    {
        HLog("Calling Stop() on first filter");
        return _filters[0]->Stop();
    }
    return false;
}

template <class T>
void HCascadedBiQuadFilter<T>::SetCoefficients(float* coefficients, int length)
{
    // Sanity check
    if( length != _firstLength )
    {
        HError("It is not possible to assign a set of coefficients of different length than the length used at construction (%d)", _firstLength);
        throw new HFilterInitializationException("Length of coefficients differs from construction length");
    }

    for( int i = 0; i < _filterCount; i++ )
    {
        _filters[i]->SetCoefficients(&coefficients[i * 5], 5);
    }
}

template <class T>
void HCascadedBiQuadFilter<T>::SetWriter(HWriter<T>* writer)
{
    _filters[_filterCount - 1]->Consumer()->SetWriter(writer);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HCascadedBiQuadFilter
template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(std::string id, HWriter<int8_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(std::string id, HWriter<uint8_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(std::string id, HWriter<int16_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(std::string id, HWriter<int32_t>* writer, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<int8_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<uint8_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<int16_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<int32_t>* consumer, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(std::string id, HReader<int8_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int8_t>* probe);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(std::string id, HReader<uint8_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<uint8_t>* probe);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(std::string id, HReader<int16_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int16_t>* probe);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(std::string id, HReader<int32_t>* reader, float* coefficients, int length, size_t blocksize, HProbe<int32_t>* probe);

template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(std::string id, HWriter<int8_t>* writer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int8_t>* probe);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(std::string id, HWriter<uint8_t>* writer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<uint8_t>* probe);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(std::string id, HWriter<int16_t>* writer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int16_t>* probe);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(std::string id, HWriter<int32_t>* writer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int32_t>* probe);

template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<int8_t>* consumer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int8_t>* probe);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<uint8_t>* consumer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<uint8_t>* probe);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<int16_t>* consumer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int16_t>* probe);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(std::string id, HWriterConsumer<int32_t>* consumer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int32_t>* probe);

template
HCascadedBiQuadFilter<int8_t>::HCascadedBiQuadFilter(std::string id, HReader<int8_t>* reader, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int8_t>* probe);

template
HCascadedBiQuadFilter<uint8_t>::HCascadedBiQuadFilter(std::string id, HReader<uint8_t>* reader, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<uint8_t>* probe);

template
HCascadedBiQuadFilter<int16_t>::HCascadedBiQuadFilter(std::string id, HReader<int16_t>* reader, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int16_t>* probe);

template
HCascadedBiQuadFilter<int32_t>::HCascadedBiQuadFilter(std::string id, HReader<int32_t>* reader, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<int32_t>* probe);


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
int HCascadedBiQuadFilter<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HCascadedBiQuadFilter<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HCascadedBiQuadFilter<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HCascadedBiQuadFilter<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

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

// SetCoefficients()
template
void HCascadedBiQuadFilter<int8_t>::SetCoefficients(float* coefficients, int length);

template
void HCascadedBiQuadFilter<uint8_t>::SetCoefficients(float* coefficients, int length);

template
void HCascadedBiQuadFilter<int16_t>::SetCoefficients(float* coefficients, int length);

template
void HCascadedBiQuadFilter<int32_t>::SetCoefficients(float* coefficients, int length);

template
void HCascadedBiQuadFilter<int8_t>::SetWriter(HWriter<int8_t>* writer);

template
void HCascadedBiQuadFilter<uint8_t>::SetWriter(HWriter<uint8_t>* writer);

template
void HCascadedBiQuadFilter<int16_t>::SetWriter(HWriter<int16_t>* writer);

template
void HCascadedBiQuadFilter<int32_t>::SetWriter(HWriter<int32_t>* writer);

//! @endcond
#endif