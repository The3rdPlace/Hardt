#ifndef __HBANDPASSBIQUAD_H
#define __HBANDPASSBIQUAD_H

template <class T>
class HBandpassBiQuad : public HBiQuad<T>
{
    public:

        HBandpassBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = 1 + alpha;
            *a1 = -2 * omegaC;
            *a2 = 1 - alpha;
            *b0 = omegaS / 2;
            *b1 = 0;;
            *b2 = -1 * (omegaS / 2);;
        }
};

#endif