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
    for( int i = 0; i < blocksize; i++)
    {
        dest[i] = _lot[_it];

        // Attempt phase correction
        float diff = abs(_flot[_it] - (float) dest[i]);
        if( diff > 0.3 )
        {
            if( _flot[_it] > _lot[_it] )
            {
                dest[i]++;
            }
            else
            {
                dest[i]--;
            }
        }

        // Increase to next lot value
        _it += _delta;

        // Past the end of the lot ?
        if( _it >= _lotSize )
        {
            _it -= _lotSize;
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

    /*
     Reasoning behind the lookup table sizing and usage
    
     *****************************************
     ** CALCULATE FOR 48KHz sample frequency **
     *****************************************
    
     Fs-max = 48KHz
     Fg-max = 24KHz
     Fg-min = 1Hz
    
     Tablesize = 48.000
    
     Number of samples per cycle, max = Fs-max / Fg-max = 48KHz / 24KHz = 2
     Number of samples per cycle, min = Fs-max / Fg-min = 48KHz / 1Hz = 48.000
    
     Pdelta-max = Tablesize / Number of samples per cycle = 48.000 / 2 = 24.000
     Pdelta-min = Tablesize / Number of samples per cycle = 48.000 / 48.000 = 1
    
     In general:
     Pdelta = Tablesize / (Fs / Fg) =  Tablesize * (Fg / Fs) = (Tablesize / Fs) * Fg
    
     Example:
    
       Fs = 48KHz
       Tablesize = 48.000
       Fg = 24KHz
       Pdelta = (48.000 / 48.000) * 24.000 = 1 * 24.000 = 24.000
    
       Fs = 48KHz
       Tablesize = 48.000
       Fg = 12KHz
       Pdelta = (48.000 / 48.000) * 12.000 = 1 * 12.000 = 12.000
    
       Fs = 48KHz
       Tablesize = 48.000
       Fg = 1Hz
       Pdelta = (48.000 / 48.000) * 1 = 1 * 1 = 1
    
     Solving for lowest frequency:
    
       Fs = 48.000
       Tablesize = 48.000
       Fg = ?
       Pdelta = 1
         (48.000 / 48.000) * Fg = 1 => 1 * Fg = 1 => Fg = 1 / 1 = 1

     Reducing the table:
    
       Fs = 48KHz
       Tablesize = 24.000
       Fg = 24KHz
       Pdelta = (24.000 / 48.000) * 24.000 = 0.5 * 24.000 = 12.000
    
       Fs = 48KHz
       Tablesize = 24.000
       Fg = 12KHz
       Pdelta = (24.000 / 48.000) * 12.000 = 0.5 * 12.000 = 6.000
    
       Fs = 48KHz
       Tablesize = 24.000
       Fg = 1Hz
       Pdelta = (24.000 / 48.000) * 1 = 0.5 * 1 = 0.5 (not possible)
    
     Solving for lowest frequency:
    
       Fs = 48.000
       Tablesize = 24.000
       Fg = ?
       Pdelta = 1
         (24.000 / 48.000) * Fg = 1 => 0.5 * Fg = 1 => Fg = 1 / 0.5 = 2
    
     Solving for lowest tablesize with Fg-min = 1KHz and Pdelta = 4 (for stability)
    
       4 = (Tablesize / 48.000) * 1.000 =>
       4 = (Tablesize * 1000) / 48.000 =>
       4 * 48.000 = Tablesize * 1000 =>
       (4 * 48.000) / 1000 = Tablesize =>
       4* (48.000 / 1000) = Tablesize =>
       4 * 48 = Tablesize =>
       192 = Tablesize
    
       in general:
    
       4 = (Tablesize / Fs) * 1.000 =>
       4 = (Tablesize * 1000) / Fs =>
       4 * Fs = Tablesize * 1000 =>
       (4 * Fs) / 1000 = Tablesize =>
       (4 / 1000) * Fs = Tablesize =>
       (4 / 1000) * Fs = Tablesize
    
     Check:
    
       (4 / 1000) * 48.000 = Tablesize
       0.004 * 48.000 = Tablesize = 192
    
     Calculate Pdelta for various frequencies:
    
       Fs = 48.000
       Tablesize = 192
       Fg = 24.000
       Pdelta = (192 / 48.000) * 24.000 =  96
    
       Fs = 48.000
       Tablesize = 192.
       Fg = 12.000
       Pdelta = (192 / 48.000) * 12.000 = 48
    
       Fs = 48.000
       Tablesize = 192
       Fg = 1000
       Pdelta = (192 / 48.000) * 1000 = 4
    
     Calculating the lot:
    
       Needing 192 samples for 2 * M_Pi radians
    
       for n=0-191 { SIN( (2 * M_Pi * n) / 192) }
    
     *****************************************
     ** CALCULATE FOR 8KHz sample frequency **
     *****************************************
    
     Calculate size of Lot:
    
       Tablesize = (4 / 1000) * Fs = Tablesize =>
       Tablesize = 0.004 * 8.000 =>
       Tablesize = 32
    
     Calculate Pdelta for various frequencies:
    
       Fs = 8.000
       Tablesize = 32
       Fg = 4.000
       Pdelta = (32 / 8.000) * 4.000 =  16
    
       Fs = 8.000
       Tablesize = 32
       Fg = 2.000
       Pdelta = (32 / 8.000) * 2.000 = 8
    
       Fs = 8.000
       Tablesize = 32
       Fg = 1000
       Pdelta = (32 / 8.000) * 1000 = 4
    
     Calculating the lot:
    
       Needing 32 samples for 2 * M_Pi radians
    
       for n=0-31 { SIN( (2 * M_Pi * n) / 32) }

     ***********************
     ** Tolerance / Error **
     ***********************

     For Fs = 48KHz and Tablesize = 192
     Find out how many frequencies can actually be synthesized

       Pdelta = (192 / 48.000) * Fg =>
       Pdelta / (192 / 48.000) = Fg =>
       (Pdelta * 48.000) / 192 = Fg =>
       Pdelta * 250 = Fg =>
       Fg = Pdelta * 250, for Pdelta = 4 -> Pdelta = 96

        1.00KHz = 4
        1.25KHz = 5
        1.50KHz = 6
        1.75KHz = 7
        2.00KHz = 8
        2.25KHz = 9
        2.50KHz = 10
        ....... = ..
       24.00KHz = 96

     So, an error of 250 Hz at maximum !!

     What about increasing the number of values in the lookup table ?

       Tablesize = 192 * 2 = 384

       Needing 192 samples for 2 * M_Pi radians

         for n=0-384 { SIN( (2 * M_Pi * n) / 384) }

       Pdelta = (Tablesize / Fs) * Fg * 2

       Pdelta-12KHz = (384 / 48.000) * 12.000 = 96

     Calculate tolerance and error now:

       Pdelta = (384 / 48.000) * Fg =>
       Pdelta / (384 / 48.000) = Fg =>
       (Pdelta * 48.000) / 384 = Fg =>
       Pdelta * 125 = Fg =>
       Fg = Pdelta * 125, for Pdelta = 8 -> Pdelta = 192

        1.000KHz = 8
        1.125KHz = 9
        1.250KHz = 10
        1.375KHz = 11
        1.500KHz = 12
        1.625KHz = 13
        1.750KHz = 14
        1.875KHz = 15
        2.000KHz = 16
        2.125KHz = 17
        2.250KHz = 18
        2.375KHz = 19
        2.500KHz = 20
        ....... = ..
       24.000KHz = 192

     We now have a tolerance of 125Hz - thats better.
     And since Pdelta = (Tablesize / Fs) * Fg:

       Pdelta-9.5kHz = (384 / 48.000) * 9500 = 76

     or

       Pdelta-10.7KHz = (384 / 48.000) * 10700 = 85.6 ~= 85 =>
       Fg = 85 * 125 = 10.625KHz (thats 75 hz off)

     So!..  Since the biggest (reduced) table we need is 192 values, we have plenty of room to increase
     that to, say 1920 (factor 10)..  How does that look ?

       Pdelta = (1920 / 48.000) * Fg =>
       Pdelta = 0.04 * Fg =>
       Pdelta / 0.04 = Fg =>
       Pdelta * 25 = Fg

       1.000KHz = 40
       1.250KHz = 50
       ... etc.

     Giving

       1.000KHz = 40
       1.025KHz = 41
       1.050KHz = 42
       ..
       1.250KHz = 50

     This reduces the tolerance to 25Hz.

     Same exaple as before:

       Pdelta-10.7KHz = (1920 / 48.000) * 10700 = 428 =>
       Fg = 428 * 25 = 10.7KHz

       Pdelta-10.650KHz = (1920 / 48.000) * 10650 = 426 =>
       Fg = 426 * 25 = 10.650KHz

       Pdelta-10.660KHz = (1920 / 48.000) * 10660 = 426.4 ~= 426
       Fg = 426 * 25 = 10.650KHz

     A 4096 points FFT at 48KHz sampling rate will have a Fdelta ~= 23.4 Hz.  So this tolerance will
     make us 'off by 1' - at worst!

     This is good enough - for now!!

     Tablesize is 1920 - for all rates, so for lower rates, we get even better precision

       Pdelta-10.7KHz = (1920 / 44.100) * 10700 = 465.8 ~= 465
       Fg = 465 * 22.7 = 10.555KHz (15 Hz off)

       Pdelta-10.7KHz = (1920 / 24.000) * 10700 = 856
       Fg = 856 * 12.5 = 10.700

     1920 values in the lookuptable, given that we use floats which normally requires 4 bytes, the
     table would require 1920 * 4 = 7.680 bytes - this is NOT a very large amount of space..!

     Since we need to, at some point, multiply with the requested amplitude and then converting to
     the output type, we might as well do that directly when calculating the lookup table.
     So, if we where to use 16 bit integer as our data type (the most often used type), the lookup table
     would take up 3840 bytes. Thats a heck of a lot smaller than many a textfile we use.

     **************
     ** Formulas **
     **************

     Tablesize = 1920 (fixed)
     Given Fs and Fg

       Pdelta = (Tablesize / Fs) * Fg

     Calculate lookup table values:

       for n=0->Tablesize-1 { SIN( (2 * M_Pi * n) / Tablesize) }

    */

    // Number of lot values
    _lotSize = rate / 10;

    // Calculate size of Lot
    _lot = new T[_lotSize];
    _flot = new float[_lotSize];
    //HLog("Created lookup table of size %d (%d bytes)", _lotSize, _lotSize * sizeof(float));

    // Cast to floats to have a controlled calculation of the lot data
    float l = _lotSize;
    float r = rate;
    float f = frequency;
    float mag = amplitude;

    // Calculate stepsize in the lot
    _delta = (l / r) * f;
    //HLog("Using delta = %f", _delta);

    // Calculate Lot value at sample n=0 -> n = N-1
    for( int i = 0; i < _lotSize; i++ )
    {
        // Calculate value for lot at index i
        float n = i;
        float fact = 2 * M_PI * (n / l);
        float out = sin(fact + phase) * mag;

        // Store the typed value
        _lot[i] = out;

        // Add the real (float) value
        _flot[i] = out;
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

template
HGenerator<float>::HGenerator(H_SAMPLE_RATE rate, int frequency, float amplitude, float phase);

template
HGenerator<double>::HGenerator(H_SAMPLE_RATE rate, int frequency, double amplitude, float phase);

// ~HGenerator
template
HGenerator<int8_t>::~HGenerator();

template
HGenerator<uint8_t>::~HGenerator();

template
HGenerator<int16_t>::~HGenerator();

template
HGenerator<int32_t>::~HGenerator();

template
HGenerator<float>::~HGenerator();

template
HGenerator<double>::~HGenerator();

// Read
template
int HGenerator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HGenerator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HGenerator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HGenerator<int32_t>::Read(int32_t* dest, size_t blocksize);

template
int HGenerator<float>::Read(float* dest, size_t blocksize);

template
int HGenerator<double>::Read(double* dest, size_t blocksize);

#endif