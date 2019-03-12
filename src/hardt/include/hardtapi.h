#ifndef __HARDTAPI_H
#define __HARDTAPI_H

#include <complex>

//#include <include/samplegenerator.h>

extern std::complex<double>* GetSamples(int sampleFreq, int duration, int amplitude, int* frequencies, int num_frequencies);

#endif
