#ifndef __HLOWPASSKAISERBESSEL_H
#define __HLOWPASSKAISERBESSEL_H

/**
    Calculate the FIR coefficients for a Kaiser-Bessel lowpass filter
*/
template <class T>
class HLowpassKaiserBessel : public HKaiserBessel<T>
{
    public:

        /** Construct a new Kaiser-Bessel filter */
        HLowpassKaiserBessel(int fstop, H_SAMPLE_RATE rate, int points, int att):
            HKaiserBessel<T>(0, fstop, rate, points, att) {}
};

#endif