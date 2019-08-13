#ifndef __HBANDPASSBIQUAD_H
#define __HBANDPASSBIQUAD_H

/**
    Calculate the normalized coefficients for a generic bandpass biquad filter.

    Given the required cutoff frequency, the sampling frequency and
    the required quality and passband gain, calculates the coefficients
    of a 2. order (biquad) IIR filter.

    This is not a filter - use this type as the generic parameter BIQUAD
    when creating a HBiQuadFilter.
*/
template <class T>
class HBandpassBiQuad : public HBiQuad<T>
{
    public:

        /** Construct a new bandpass biquad */
        HBandpassBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        /** Calculate filter coefficients */
        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = 1 + alpha;
            *a1 = -2 * omegaC;
            *a2 = 1 - alpha;
            *b0 = omegaS / 2;
            *b1 = 0;
            *b2 = -1 * (omegaS / 2);
        }

        /** Calculate and return the normalized filter coefficients */
        float* Calculate()
        {
            return HBiQuad<T>::Calculate();
        }
};

#endif