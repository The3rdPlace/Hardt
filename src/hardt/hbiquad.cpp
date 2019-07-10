#ifndef __HBIQUAD_CPP
#define __HBIQUAD_CPP

#include "hbiquad.h"

float HBiQuad::biquadParameters[6];
float HBiQuad::normalizedBiquadParameters[5];

float HBiQuad::omegaC;
float HBiQuad::omegaS;
float HBiQuad::alpha;
float HBiQuad::A;
float HBiQuad::beta;

void HBiQuad::Initialize(float fCutOff, float rate, float quality, float gain)
{
    float omegaCutOff = 2 * M_PI * (fCutOff / rate);

    // Basic intermediate values
    omegaC = cos(omegaCutOff);
    omegaS = sin(omegaCutOff);
    alpha = omegaS / (2 * quality);

    // Only used for shelving and peaking filters
    A = sqrt(pow(10, (gain / 20)));
    beta = sqrt(A) / quality;
}

float* HBiQuad::Normalize()
{
    if( a0 != 1 )
    {
        normalizedBiquadParameters[0] = a1 / a0;
        normalizedBiquadParameters[1] = a2 / a0;
        normalizedBiquadParameters[2] = b0 / a0;
        normalizedBiquadParameters[3] = b1 / a0;
        normalizedBiquadParameters[4] = b2 / a0;
    }
    else
    {
        normalizedBiquadParameters[0] = a1;
        normalizedBiquadParameters[1] = a2;
        normalizedBiquadParameters[2] = b0;
        normalizedBiquadParameters[3] = b1;
        normalizedBiquadParameters[4] = b2;
    }
    return normalizedBiquadParameters;
}

#endif