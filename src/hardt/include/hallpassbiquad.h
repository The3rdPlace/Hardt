#ifndef __HALLPASSBIQUAD_H
#define __HALLPASSBIQUAD_H

/**
    Calculate the normalized coefficients for a generic allpass biquad filter.

    Given the required cutoff frequency, the sampling frequency and
    the required quality and passband gain, calculates the coefficients
    of a 2. order (biquad) IIR filter.

    This is not a filter - use this type as the generic parameter BIQUAD
    when creating a HBiQuadFilter.
*/
template <class T>
class HAllpassBiQuad : public HBiQuad<T>
{
    public:

        /**
            Create a new allpass filter specification object
        */
        HAllpassBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        /**
            Calculate non-normalized coefficients for allpass filter.
        */
        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = 1 + alpha;
            *a1 = -2 * omegaC;
            *a2 = 1 - alpha;
            *b0 = 1 - alpha;
            *b1 = -2 * omegaC;
            *b2 = 1 + alpha;
        }

        /**
            Call calculate on the base type - which then calls our calculate with
            the temporary variables omegaC, omegaS, alpha, A and beta set.
        */
        float* Calculate()
        {
            return HBiQuad<T>::Calculate();
        }
};

#endif