#ifndef __HGENERATOR_CPP
#define __HGENERATOR_CPP

#include "hgenerator.h"
#include <cstring>

template <class T>
HGenerator<T>::HGenerator(H_SAMPLE_RATE rate, size_t blocksize, int frequency, int numberOfFrequencies, int phase)
{
    // Calculate how many samples is needed for a number of full cycles
    int samplesPerCycle = (int) rate / frequency;

    // Get a temporary table for sample value calculations
    T table[samplesPerCycle];

    // Todo: Calculate lookup table content for one cycle
    table[0] = 0; // ...and so forth

    // Try to allocate and store as many cycles as possible so that we can get
    // as few transfers on each read as possible
    int blocks = blocksize / samplesPerCycle;
    if( blocks > 0 )
    {
        // How many full cycles can we have ?
        _length = blocks * samplesPerCycle;
        _lot = new T[_length];
        for( int block = 0; block < blocks; block++ )
        {
            memcpy((void*) &_lot[block], (void*) table, samplesPerCycle);
        }
    }
    else
    {
        // Store one full cycle
        _length = samplesPerCycle;
        _lot = new T[_length];
        memcpy((void*) _lot, (void*) table, samplesPerCycle);
    }

    // Begin reading samples out from the beginning of the array
    next = 0;
}

template <class T>
HGenerator<T>::~HGenerator()
{
    delete _lot;
}

template <class T>
void HGenerator<T>::GetSamples(T* dest, size_t blocksize)
{
    // Todo: Get <blocksize> consecutive samples
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HGenerator
template
HGenerator<int8_t>::HGenerator(H_SAMPLE_RATE rate, size_t blocksize, int frequency, int numberOfFrequencies, int phase);

template
HGenerator<uint8_t>::HGenerator(H_SAMPLE_RATE rate, size_t blocksize, int frequency, int numberOfFrequencies, int phase);

template
HGenerator<int16_t>::HGenerator(H_SAMPLE_RATE rate, size_t blocksize, int frequency, int numberOfFrequencies, int phase);

template
HGenerator<int32_t>::HGenerator(H_SAMPLE_RATE rate, size_t blocksize, int frequency, int numberOfFrequencies, int phase);

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