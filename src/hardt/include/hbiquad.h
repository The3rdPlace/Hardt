#ifndef __HBIQUAD_H
#define __HBIQUAD_H

#define a0 biquadParameters[0]
#define a1 biquadParameters[1]
#define a2 biquadParameters[2]
#define b0 biquadParameters[3]
#define b1 biquadParameters[4]
#define b2 biquadParameters[5]

class HBiQuad
{
    public:

        static float biquadParameters[6];
        static float normalizedBiquadParameters[5];

        static float omegaC;
        static float omegaS;
        static float alpha;
        static float A;
        static float beta;

    protected:

        static void Initialize(float fCutOff, float rate, float quality, float gain);
        static float* Normalize();
};

#endif