#ifndef __HBANDPASSKAISERBESSEL_H
#define __HBANDPASSKAISERBESSEL_H

/**
    Calculate the FIR coefficients for a Kaiser-Bessel bandpass filter
*/
template <class T>
class HBandpassKaiserBessel : public HKaiserBessel<T>
{
    public:

        /** Construct a new Kaiser-Bessel filter */
        HBandpassKaiserBessel(int fstart, int fstop, H_SAMPLE_RATE rate, int points, int att):
            HKaiserBessel<T>(fstart, fstop, (float) rate, points, att) {}
};

#endif