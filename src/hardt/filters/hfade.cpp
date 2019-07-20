#ifndef __HFADE_CPP
#define __HFADE_CPP

#include "hfade.h"

template <class T>
HFade<T>::HFade(HReader<T>* reader, int initialLevel, int samples, bool fade, size_t blocksize):
    HGain<T>(reader, 1, blocksize),
    _level(initialLevel),
    _samples(samples),
    _direction(initialLevel > 50), // if initial level is above 50, fade up, otherwise down
    _fade(fade)
{
    HLog("HFade(HReader*, %d, %d, %d)", initialLevel, samples, fade);
    Init();
}

template <class T>
HFade<T>::HFade(HWriter<T>* writer, int initialLevel, int samples, bool fade, size_t blocksize):
    HGain<T>(writer, 1, blocksize),
    _level(initialLevel),
    _samples(samples),
    _direction(initialLevel >= 50), // if initial level is above 50, fade up, otherwise down
    _fade(fade)
{
    HLog("HFade(HWriter*, %d, %d, %d)", initialLevel, samples, fade);
    Init();
}

template <class T>
void HFade<T>::Init()
{
    // Check initial fading
    if( _level <= 0 && _fade )
    {
        _direction = true;
        _level = 0;
        _counter = 0;
    }
    else if( _level >= 100 && _fade )
    {
        _direction = false;
        _level = 100;
        _counter = 100 * _samples;
    }
    else
    {
        if( _level > 100 )
        {
            _level = 100;
        }
        else if( _level < 0 )
        {
            _level = 0;
        }
        _counter = _level * _samples;
        _fade = 0;
    }

    // Set current gain
    HGain<T>::SetGain((float) _level / (float) 100);
}

template <class T>
HFade<T>::~HFade()
{
    HLog("~HFade()");
}

template <class T>
void HFade<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // If we are fading, calculate new level
    if( _fade )
    {
        // Update the counter and calculate current level
        if( _direction )
        {
            _counter += blocksize;

        }
        else
        {
            _counter -= blocksize;

        }
        _level = _counter / _samples;

        // Check if we have reached top or bottom
        if( _direction && _level >= 100 )
        {
            _level = 100;
            _counter = 0;
            _fade = false;
            HGain<T>::SetGain(1);
        }
        else if( !_direction && _level <= 0 )
        {
            _level = 0;
            _counter = 0;
            _fade = false;
            HGain<T>::SetGain(0);
        }
        else
        {
            HGain<T>::SetGain((float) _level / (float) 100);
        }
    }

    // Run the normal gain filter
    HGain<T>::Filter(src, dest, blocksize);
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HFade
template
HFade<int8_t>::HFade(HWriter<int8_t>* writer, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<uint8_t>::HFade(HWriter<uint8_t>* writer, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<int16_t>::HFade(HWriter<int16_t>* writer, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<int32_t>::HFade(HWriter<int32_t>* writer, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<int8_t>::HFade(HReader<int8_t>* reader, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<uint8_t>::HFade(HReader<uint8_t>* reader, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<int16_t>::HFade(HReader<int16_t>* reader, int initialLevel, int samples, bool fade, size_t blocksize);

template
HFade<int32_t>::HFade(HReader<int32_t>* reader, int initialLevel, int samples, bool fade, size_t blocksize);

// ~HFade()
template
HFade<int8_t>::~HFade();

template
HFade<uint8_t>::~HFade();

template
HFade<int16_t>::~HFade();

template
HFade<int32_t>::~HFade();

// Filter
template
void HFade<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HFade<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HFade<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HFade<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

#endif