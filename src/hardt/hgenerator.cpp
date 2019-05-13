#ifndef __HGENERATOR_CPP
#define __HGENERATOR_CPP

#include "hgenerator.h"
//#include <cstring>

template <class T>
HGenerator<T>::HGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase)
{

    // Calculate the amount of samples that we need. And then divide by 4
    // since we only need 1/4 of the sine, the rest can be constructed by changing sign.
    // Please note that for frequencies that produce decimal values when divided up into
    // the samplerate, we can only approximate the frequency - this is accepted for all
    // purposes for which this lib. is intended
    HLog("HGenerator(rate = %d, frequency = %d, phase = %f)", rate, frequency, phase);

    //int numSamples = (int) rate;
    //HLog("numSamples = %d", numSamples);

    int lotSize = rate / 4;
    HLog("Creating lookup table of size %d (%d bytes)", lotSize, lotSize * sizeof(T));
    _lot = new T[lotSize];

    // Calculate lookup table
    for( int i = 0; i < lotSize; i++ )
    {
        _lot[i] = amplitude * sin((((2 * M_PI * frequency) / rate) * i) + phase);
    }

    // Calculate quadrant markers
    _q1 = rate / 4;
    _q2 = (rate / 4) * 2;
    _q3 = (rate / 4) * 3;
    _q4 = (rate / 4) * 4;
    HLog("Quadrant markers (1., 2., 3., 4.) = %d, %d, %d, %d", _q1, _q2, _q3, _q4);

    // Initial sample position
    _it = 0;
}

template <class T>
HGenerator<T>::~HGenerator()
{
    HLog("~HGenerator()");
    delete _lot;
}

template <class T>
void HGenerator<T>::GetSamples(T* dest, size_t blocksize)
{
    for( int i = 0; i < blocksize; i++ )
    {
        // Select quadrant, then copy and possibly alter the value
        if( _it < _q1 )
        {
            dest[i] = _lot[_it];
        }
        else if( _it < _q2 )
        {
            int index = (-1 * _it) + _q2;
            dest[i] = _lot[index];
        }
        else if( _it < _q3 )
        {
            int index = _it - _q2;
            dest[i] = -1 * _lot[index];
        }
        else
        {
            int index = _q1 - _it + _q3;
            dest[i] = -1 * _lot[index];
        }

        // Next sample
        _it++;

        // Roll over to first sample in LOT
        if( _it >= _q4 )
        {
            _it = 0;
        }
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HGenerator
template
HGenerator<int8_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude, float phase);

template
HGenerator<uint8_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude, float phase);

template
HGenerator<int16_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude, float phase);

template
HGenerator<int32_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude, float phase);

// ~HGenerator
template
HGenerator<int8_t>::~HGenerator();

template
HGenerator<uint8_t>::~HGenerator();

template
HGenerator<int16_t>::~HGenerator();

template
HGenerator<int32_t>::~HGenerator();

// GetSamples
template
void HGenerator<int8_t>::GetSamples(int8_t* dest, size_t blocksize);

template
void HGenerator<uint8_t>::GetSamples(uint8_t* dest, size_t blocksize);

template
void HGenerator<int16_t>::GetSamples(int16_t* dest, size_t blocksize);

template
void HGenerator<int32_t>::GetSamples(int32_t* dest, size_t blocksize);

#endif