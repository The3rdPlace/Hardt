#ifndef __HHIGHSHELVINGBIQUAD_H
#define __HHIGHSHELVINGBIQUAD_H

/**
    Calculate the normalized coefficients for a generic high shelving biquad filter.

    Given the required cutoff frequency, the sampling frequency and
    the required quality and passband gain, calculates the coefficients
    of a 2. order (biquad) IIR filter.

    This is not a filter - use this type as the generic parameter BIQUAD
    when creating a HBiQuadFilter.
*/
template <class T>
class HHighShelvingBiQuad : public HBiQuad<T>
{
    public:

        HHighShelvingBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = ((A + 1) - ((A - 1) * omegaC) + (beta * omegaS));
            *a1 = 2 * ((A - 1) - ((A + 1) * omegaC));
            *a2 = ((A + 1) - ((A - 1) * omegaC) - (beta * omegaS));
            *b0 = A * ((A + 1) + ((A-1) * omegaC) + (beta * omegaS));
            *b1 = -2 * A * ((A - 1) + ((A + 1) * omegaC));
            *b2 = A * ( (A + 1) + ((A-1) * omegaC) - (beta * omegaS));
        }

        float* Calculate()
        {
            return HBiQuad<T>::Calculate();
        }
};

#endif