#ifndef __HBANDSTOPKAISERBESSEL_H
#define __HBANDSTOPKAISERBESSEL_H

/**
    Calculate the FIR coefficients for a Kaiser-Bessel bandstop filter
*/
template <class T>
class HBandstopKaiserBessel : public HKaiserBessel<T>
{
    public:

        /** Construct a new Kaiser-Bessel filter */
        HBandstopKaiserBessel(int fstart, int fstop, H_SAMPLE_RATE rate, int points, int att):
            HKaiserBessel<T>(fstart, fstop, (float) rate, points, att) {}

        virtual void Calculate(float* coefficients, int points) {
            // Todo: modify coefficients
        };
};

#endif