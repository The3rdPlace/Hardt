#ifndef __HBANDPASSBIQUAD_H
#define __HBANDPASSBIQUAD_H

/**
    Specific implementation of a generic IIR allpass filter
*/
template <class T>
class HBandpassBiQuad : public HBiQuad<T>
{
    public:

        /**
            Create a new bandpass filter specification object
        */
        HBandpassBiQuad(float fCutOff, float rate, float quality, float gain):
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
            *b0 = omegaS / 2;
            *b1 = 0;
            *b2 = -1 * (omegaS / 2);
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