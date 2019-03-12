#ifndef __SAMPLEGENERATOR_H
#define __SAMPLEGENERATOR_H

#include <complex>

class Generator
{
    public:
        std::complex<double>* GetSamples(int sampleFreq, int duration, int amplitude, int* frequencies, int num_frequencies);
};

#endif
