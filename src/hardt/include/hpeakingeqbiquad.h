#ifndef __HPEAKINGEQBIQUAD_H
#define __HPEAKINGEQBIQUAD_H

template <class T>
class HPeakingEQBiQuad : public HBiQuad<T>
{
    public:

        HPeakingEQBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = 1 + (alpha / A);
            *a1 = -2 * omegaC;
            *a2 = 1 - (alpha / A);
            *b0 = 1 + (alpha * A);
            *b1 = -2 * omegaC;
            *b2 = 1 - (alpha * A);
        }
};

#endif