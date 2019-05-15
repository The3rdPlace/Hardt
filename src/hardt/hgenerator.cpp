#ifndef __HGENERATOR_CPP
#define __HGENERATOR_CPP

#include "hgenerator.h"

template <class T>
HGenerator<T>::HGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase):
    _lot(NULL)
{
    HLog("HGenerator(rate = %d, frequency = %d, phase = %f)", rate, frequency, phase);
    Calculate(rate, frequency, amplitude, phase);
}

template <class T>
HGenerator<T>::~HGenerator()
{
    HLog("~HGenerator()");
    delete _lot;
}

template <class T>
int HGenerator<T>::Read(T* dest, size_t blocksize)
{
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = _lot[_it++];
        if( _it >= 16 )
        {
            _it = 0;
        }
    }

    return blocksize;
}

template <class T>
void HGenerator<T>::Calculate(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase)
{
    // Release previous lookup table ?
    if( _lot != NULL )
    {
        HLog("Releasing previous lookup table");
        delete _lot;
    }

    // How many distinct samples can we have per cycle
    int samplesPerCycle = rate / frequency;

    // Create lookup table
    HLog("Creating lookup table of size %d (%d bytes)", samplesPerCycle, samplesPerCycle * sizeof(T));
    _lot = new T[samplesPerCycle];

    // Calculate lookup table
    for( int i = 0; i < samplesPerCycle; i++ )
    {
        _lot[i] = amplitude * sin((((2 * M_PI * frequency) / rate) * i) + phase);
        HLog("lot[%d] = %d", i, _lot[i]);
    }

    // Initial sample position
    _it = 0;
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

// Read
template
int HGenerator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HGenerator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HGenerator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HGenerator<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif