#ifndef __HBIQUAD_CPP
#define __HBIQUAD_CPP

#include "hbiquad.h"

float HBiQuad::biquadParameters[6];
float HBiQuad::normalizedBiquadParameters[5];

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