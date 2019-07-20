#ifndef __HLOWSHELVINGBIQUAD_H
#define __HLOWSHELVINGBIQUAD_H

template <class T>
class HLowShelvingBiQuad : public HBiQuad<T>
{
    public:

        HLowShelvingBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = ((A + 1) + ((A - 1) * omegaC) + (beta * omegaS));
            *a1 = -2 * ((A - 1) + ((A + 1) * omegaC));
            *a2 = ((A + 1) + ((A - 1) * omegaC) - (beta * omegaS));
            *b0 = A * ((A + 1) - ((A-1) * omegaC) + (beta * omegaS));
            *b1 = 2 * A * ((A - 1) - ((A + 1) * omegaC));
            *b2 = A * ( (A + 1) - ((A-1) * omegaC) - (beta * omegaS));
        }

        float* Calculate()
        {
            return HBiQuad<T>::Calculate();
        }
};

#endif