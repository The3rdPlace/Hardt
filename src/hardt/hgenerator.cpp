#ifndef __HGENERATOR_CPP
#define __HGENERATOR_CPP

#include "hgenerator.h"
#include <cstring>

template <class T>
HGenerator<T>::HGenerator(H_SAMPLE_RATE rate, int frequency, int phase)
{

    // Calculate the amount of samples that we need. And then divide by 4
    // since we only need 1/4 of the sine, the rest can be constructed by changing sign.
    // Please note that for frequencies that produce decimal values when divided up into
    // the samplerate, we can only approximate the frequency - this is accepted for all
    // purposes for which this lib. is intended
    HLog("Frequency = %d", frequency);
    HLog("Sample rate = %d", rate);

    int numSamples = (int) rate;
    HLog("numSamples = %d", numSamples);

    int lotSize = numSamples / 4;
    HLog("lotSize = %d", lotSize);

    // Calculate lookup table
    T amplitude = 127;
    _lot = new T[lotSize];
    for( int i = 0; i < lotSize; i++ )
    {
        _lot[i] = amplitude * sin(((2 * M_PI * frequency) / rate) * i);
        HLog("t[%d] = %d", i, _lot[i]);
    }

    // Calculate quadrant markers
    _q1 = numSamples / 4;
    _q2 = (numSamples / 4) * 2;
    _q3 = (numSamples / 4) * 3;
    _q4 = (numSamples / 4) * 4;
    HLog("q1 = %d, q2 = %d, q3 = %d, q4 = %d", _q1, _q2, _q3, _q4);

    _it = 0;

}

template <class T>
HGenerator<T>::~HGenerator()
{
    delete _lot;
}

template <class T>
void HGenerator<T>::GetSamples(T* dest, size_t blocksize)
{
    for( int i = 0; i < blocksize; i++ )
    {
        if( _it < _q1 )
        {
            dest[i] = _lot[_it];
            HLog("1 %d _lot[%d] = %d", _it, _it, dest[i]);
        }
        else if( _it < _q2 )
        {
            int index = (-1 * _it) + _q2;
            dest[i] = _lot[index];
            HLog("2 %d _lot[%d] = %d", _it, index, dest[i]);
        }
        else if( _it < _q3 )
        {
            int index = _it - _q2;
            dest[i] = -1 * _lot[index];
            HLog("3 %d _lot[%d] = %d", _it, index, dest[i]);
        }
        else
        {
            int index = _q1 - _it + _q3;
            dest[i] = -1 * _lot[index];
            HLog("4 %d _lot[%d] = %d", _it, index, dest[i]);
        }

        _it++;

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
HGenerator<int8_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int phase);

template
HGenerator<uint8_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int phase);

template
HGenerator<int16_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int phase);

template
HGenerator<int32_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int phase);

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