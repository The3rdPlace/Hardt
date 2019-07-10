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

    protected:

        static float* Normalize();
};

#endif