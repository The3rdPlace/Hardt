#ifndef __HSIGNALLEVEL_CPP
#define __HSIGNALLEVEL_CPP

#include "hsignallevel.h"

template <class T>
HSignalLevel<T>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref):
    HOutput<T, HSignalLevelResult>(writer),
    _blocksReceived(0),
    _skip(skip),
    _ref(ref)
{
    HLog("HSignalLevel(...)");

    _factor = (float) (std::numeric_limits<T>::is_signed ? std::numeric_limits<T>::max() : std::numeric_limits<T>::max() / 2) / 508;
}

template <class T>
int HSignalLevel<T>::Output(T* src, size_t blocksize)
{
    // Skip this block ?
    if( _blocksReceived++ < _skip )
    {
        return blocksize;
    }

    // Analyze block - disable warnings about using abs() for unsigned types
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wabsolute-value"
    T max = 0;
    for( int i = 0; i < blocksize; i++ )
    {
        if( abs(src[i]) > max )
        {
            max = abs(src[i]);
        }
    }
    #pragma GCC diagnostic pop

    // Store collected information
    _result.Max = max;

    // Calculate signal level
    _result.Db = 20 * log10((float) ceil((_result.Max == 0 ? 0.25 : _result.Max) / (float) _factor));
    _result.S = ((_result.Db) / 6);

    // Adjust DB readings after ref
    _result.Db -= _ref;

    // Send results
    HOutput<T, HSignalLevelResult>::Ready(&_result, 1);

    // Reset block counter
    _blocksReceived = 0;

    // We took the entire window
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HGoertzelFilter
template
HSignalLevel<int8_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<uint8_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<int16_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<int32_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

// Output
template
int HSignalLevel<int8_t>::Output(int8_t* src, size_t size);

template
int HSignalLevel<uint8_t>::Output(uint8_t* src, size_t size);

template
int HSignalLevel<int16_t>::Output(int16_t* src, size_t size);

template
int HSignalLevel<int32_t>::Output(int32_t* src, size_t size);

#endif
