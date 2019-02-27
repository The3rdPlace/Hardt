#ifndef __SAMPLEGENERATOR_H
#define __SAMPLEGENERATOR_H

#include <hardt.h>

#include <math.h>
#include <complex>

namespace Hardt
{
    class SampleGenerator
    {
        public:

            static std::complex<double>* GetSamples(int sampleFreq, int duration, int amplitude, int* frequencies, int num_frequencies);
    };
}

#endif
