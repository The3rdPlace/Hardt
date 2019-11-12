#ifndef __HSIGNALLEVEL_CPP
#define __HSIGNALLEVEL_CPP

#include "hsignallevel.h"

template <class T>
HSignalLevel<T>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int average, int ref):
    HOutput<T, HSignalLevelResult>(writer),
    _ref(ref),
    _avg(NULL),
    _avgPos(0),
    _avgCount(average > 0 ? average : 1)
{
    HLog("HSignalLevel(...)");
    Init();
}

template <class T>
HSignalLevel<T>::HSignalLevel(HWriterConsumer<T>* consumer, int average, int ref):
    HOutput<T, HSignalLevelResult>(consumer),
    _ref(ref),
    _avg(NULL),
    _avgPos(0),
    _avgCount(average > 0 ? average : 1)
{
    HLog("HSignalLevel(...)");
    Init();
}

template <class T>
void HSignalLevel<T>::Init()
{
    _avg = new T[_avgCount];
    memset((void*) _avg, 0, sizeof(T) * _avgCount);

    _factor = (float) (std::numeric_limits<T>::is_signed ? std::numeric_limits<T>::max() : std::numeric_limits<T>::max() / 2) / 508;
}

template <class T>
HSignalLevel<T>::~HSignalLevel()
{
    if( _avg != NULL )
    {
        delete _avg;
    }
}

template <class T>
int HSignalLevel<T>::Output(T* src, size_t blocksize)
{
    // Analyze block - disable warnings about using abs() for unsigned types
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wabsolute-value"
    T max = 0;
    T min = std::numeric_limits<T>::max();
    for( int i = 0; i < blocksize; i++ )
    {
        if( abs(src[i]) > max )
        {
            max = abs(src[i]);
        }
        if( abs(src[i]) < min )
        {
            min = abs(src[i]);
        }
    }
    #pragma GCC diagnostic pop

    // Adjust min and max for unsigned types
    if( !std::numeric_limits<T>::is_signed )
    {
        max -= floor(std::numeric_limits<T>::max() / 2);
        min -= floor(std::numeric_limits<T>::max() / 2);
    }

    // Store this max
    _avg[_avgPos++] = max;

    // If we have seen enough samples, calculate the average and execute the callback
    if( _avgPos >= _avgCount )
    {
        T avg = 0;
        for( int i = 0; i < _avgCount; i++ )
        {
            avg += _avg[i];
        }
        avg /= _avgCount;

        // Store collected information
        _result.Max = max;
        _result.Min = min;
        _result.Avg = avg;

        // Calculate signal level
        _result.Db = 20 * log10((float) ceil((_result.Max == 0 ? 0.25 : _result.Max) / (float) _factor));
        _result.AvgDb = 20 * log10((float) ceil((_result.Avg == 0 ? 0.25 : _result.Avg) / (float) _factor));
        _result.S = ((_result.Db) / 6);
        _result.AvgS = ((_result.AvgDb) / 6);

        // Adjust DB readings after ref
        _result.Db -= _ref;
        _result.AvgDb -= _ref;

        // Send results
        HOutput<T, HSignalLevelResult>::Ready(&_result, 1);

        // Reset block counter
        _avgPos = 0;
    }

    // We took the entire window
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HGoertzelFilter
template
HSignalLevel<int8_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<uint8_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<int16_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<int32_t>::HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref);

template
HSignalLevel<int8_t>::HSignalLevel(HWriterConsumer<int8_t>* consumer, int skip, int ref);

template
HSignalLevel<uint8_t>::HSignalLevel(HWriterConsumer<uint8_t>* consumer, int skip, int ref);

template
HSignalLevel<int16_t>::HSignalLevel(HWriterConsumer<int16_t>* consumer, int skip, int ref);

template
HSignalLevel<int32_t>::HSignalLevel(HWriterConsumer<int32_t>* consumer, int skip, int ref);

// ~HSignalLevel
template
HSignalLevel<int8_t>::~HSignalLevel();

template
HSignalLevel<uint8_t>::~HSignalLevel();

template
HSignalLevel<int16_t>::~HSignalLevel();

template
HSignalLevel<int32_t>::~HSignalLevel();

// Output
template
int HSignalLevel<int8_t>::Output(int8_t* src, size_t size);

template
int HSignalLevel<uint8_t>::Output(uint8_t* src, size_t size);

template
int HSignalLevel<int16_t>::Output(int16_t* src, size_t size);

template
int HSignalLevel<int32_t>::Output(int32_t* src, size_t size);

//! @endcond
#endif
