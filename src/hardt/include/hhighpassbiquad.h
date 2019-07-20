#ifndef __HHIGHPASSBIQUAD_H
#define __HHIGHPASSBIQUAD_H

template <class T>
class HHighpassBiQuad : public HBiQuad<T>
{
    public:

        HHighpassBiQuad(float fCutOff, float rate, float quality, float gain):
            HBiQuad<T>(fCutOff, rate, quality, gain)
        {}

        void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2)
        {
            *a0 = 1 + alpha;
            *a1 = -2 * omegaC;
            *a2 = 1 - alpha;
            *b0 = (1 + omegaC) / 2;
            *b1 = - 1 * (1 + omegaC);
            *b2 = (1 + omegaC) / 2;
        }

        float* Calculate()
        {
            return HBiQuad<T>::Calculate();
        }
};

#endif