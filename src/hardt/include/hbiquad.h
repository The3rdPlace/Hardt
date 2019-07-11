#ifndef __HBIQUAD_H
#define __HBIQUAD_H

template <class T>
class HBiQuad
{
    private:

        float biquadParameters[6];
        float normalizedBiquadParameters[5];

        float omegaC;
        float omegaS;
        float alpha;
        float A;
        float beta;

        virtual void Calculate(float omegaC, float omegaS, float alpha, float A, float beta, float *a0, float* a1, float* a2, float* b0, float* b1, float* b2) = 0;
        float* Normalize();

    protected:

        HBiQuad(float fCutOff, float rate, float quality, float gain);

    public:

        float* Calculate();
};

#endif