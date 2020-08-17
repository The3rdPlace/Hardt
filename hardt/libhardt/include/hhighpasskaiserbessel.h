#ifndef __HHIGHPASSKAISERBESSEL_H
#define __HHIGHPASSKAISERBESSEL_H

#include "hkaiserbessel.h"

/**
    Calculate the FIR coefficients for a Kaiser-Bessel highpass filter
*/
template <class T>
class HHighpassKaiserBessel : public HKaiserBessel<T>
{
    public:

        /** Construct a new Kaiser-Bessel filter */
        HHighpassKaiserBessel(int fstart, H_SAMPLE_RATE rate, int points, int att):
            HKaiserBessel<T>(fstart, rate / 2, rate, points, att) {
        }
};

#endif