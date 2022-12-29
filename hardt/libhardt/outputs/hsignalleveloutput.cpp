#ifndef __HSIGNALLEVELOUTPUT_CPP
#define __HSIGNALLEVELOUTPUT_CPP

#include <cstdint>
#include <cstdio>

#include "hsignalleveloutput.h"

template <class T>
HSignalLevelOutput<T>::HSignalLevelOutput(std::string id, HWriter<HSignalLevelResult>* writer, int average, int ref, int scale):
    HOutput<T, HSignalLevelResult>(id, writer),
    _ref(ref),
    _scale(scale),
    _avg(NULL),
    _avgSum(NULL),
    _avgPos(0),
    _avgCount(average > 0 ? average : 1)
{
    HLog("HSignalLevel(...)");
    Init();
}

template <class T>
HSignalLevelOutput<T>::HSignalLevelOutput(std::string id, HWriterConsumer<T>* consumer, int average, int ref, int scale):
    HOutput<T, HSignalLevelResult>(id, consumer),
    _ref(ref),
    _scale(scale),
    _avg(NULL),
    _avgSum(NULL),
    _avgPos(0),
    _avgCount(average > 0 ? average : 1)
{
    HLog("HSignalLevelOutput(...)");
    Init();
}

template <class T>
void HSignalLevelOutput<T>::Init()
{
    _avg = new T[_avgCount];
    _avgSum = new double[_avgCount];
    memset((void*) _avg, 0, sizeof(T) * _avgCount);
    memset((void*) _avgSum, 0, sizeof(double) * _avgCount);

    _factor = (float) (std::numeric_limits<T>::is_signed ? std::numeric_limits<T>::max() : std::numeric_limits<T>::max() / 2) / 508;
}

template <class T>
HSignalLevelOutput<T>::~HSignalLevelOutput()
{
    if( _avg != NULL )
    {
        delete _avg;
    }
    if( _avgSum != NULL )
    {
        delete _avgSum;
    }
}

template <class T>
int HSignalLevelOutput<T>::Output(T* src, size_t blocksize)
{
    // Analyze block
    int max = 0;
    int min = std::numeric_limits<T>::max();
    double sum = 0;
    for( int i = 0; i < blocksize; i++ )
    {
        if( abs((int) src[i]) > max )
        {
            max = abs((int) src[i]);
        }
        if( abs(src[i]) < min )
        {
            min = abs((int) src[i]);
        }

        sum += abs((int) src[i]);
    }

    // Scaling
    max *= _scale;
    min *= _scale;

    // Adjust min and max for unsigned types
    if( !std::numeric_limits<T>::is_signed )
    {
        max -= floor(std::numeric_limits<T>::max() / 2);
        min -= floor(std::numeric_limits<T>::max() / 2);
    }

    // Store this max
    _avg[_avgPos] = max;
    _avgSum[_avgPos++] = sum;

    // If we have seen enough samples, calculate the average and execute the callback
    if( _avgPos >= _avgCount )
    {
        int avg = 0;
        double avgSum = 0;
        for( int i = 0; i < _avgCount; i++ )
        {
            avg += _avg[i];
            avgSum += _avgSum[i];
        }
        avg /= _avgCount;
        avgSum /= _avgCount;

        // Store collected information
        _result.Max = max;
        _result.Min = min;
        _result.Avg = avg;
        _result.Sum = avgSum;

        // Calculate signal level
        _result.Db = 20 * log10((float) ceil((_result.Max == 0 ? 0.25 : _result.Max) / (float) _factor));
        _result.AvgDb = 20 * log10((float) ceil((avg == 0 ? 0.25 : avg) / (float) _factor));
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
HSignalLevelOutput<int8_t>::HSignalLevelOutput(std::string id, HWriter<HSignalLevelResult>* writer, int skip, int ref, int scale);

template
HSignalLevelOutput<uint8_t>::HSignalLevelOutput(std::string id, HWriter<HSignalLevelResult>* writer, int skip, int ref, int scale);

template
HSignalLevelOutput<int16_t>::HSignalLevelOutput(std::string id, HWriter<HSignalLevelResult>* writer, int skip, int ref, int scale);

template
HSignalLevelOutput<int32_t>::HSignalLevelOutput(std::string id, HWriter<HSignalLevelResult>* writer, int skip, int ref, int scale);

template
HSignalLevelOutput<int8_t>::HSignalLevelOutput(std::string id, HWriterConsumer<int8_t>* consumer, int skip, int ref, int scale);

template
HSignalLevelOutput<uint8_t>::HSignalLevelOutput(std::string id, HWriterConsumer<uint8_t>* consumer, int skip, int ref, int scale);

template
HSignalLevelOutput<int16_t>::HSignalLevelOutput(std::string id, HWriterConsumer<int16_t>* consumer, int skip, int ref, int scale);

template
HSignalLevelOutput<int32_t>::HSignalLevelOutput(std::string id, HWriterConsumer<int32_t>* consumer, int skip, int ref, int scale);

// ~HSignalLevel
template
HSignalLevelOutput<int8_t>::~HSignalLevelOutput();

template
HSignalLevelOutput<uint8_t>::~HSignalLevelOutput();

template
HSignalLevelOutput<int16_t>::~HSignalLevelOutput();

template
HSignalLevelOutput<int32_t>::~HSignalLevelOutput();

// Output
template
int HSignalLevelOutput<int8_t>::Output(int8_t* src, size_t size);

template
int HSignalLevelOutput<uint8_t>::Output(uint8_t* src, size_t size);

template
int HSignalLevelOutput<int16_t>::Output(int16_t* src, size_t size);

template
int HSignalLevelOutput<int32_t>::Output(int32_t* src, size_t size);

//! @endcond
#endif
