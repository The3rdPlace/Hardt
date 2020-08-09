#ifndef __HBANDSTOPKAISERBESSEL_H
#define __HBANDSTOPKAISERBESSEL_H

/**
    Calculate the FIR coefficients for a Kaiser-Bessel bandstop filter
*/
template <class T>
class HBandstopKaiserBessel : public HKaiserBessel<T>
{

    private:

        void Modify(float* coefficients, int points) {

            int Np = ((points - 1) / 2);

            coefficients[Np] = 1- coefficients[Np];
            for( int i = 0; i < Np; i++ ) {
                coefficients[i] *= -1;
                coefficients[Np + 1 + i] *= -1;
            }

        };

    public:

        /** Construct a new Kaiser-Bessel filter */
        HBandstopKaiserBessel(int fstart, int fstop, H_SAMPLE_RATE rate, int points, int att):
            HKaiserBessel<T>(fstart, fstop, rate, points, att) {}
};

#endif