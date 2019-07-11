#ifndef __HBIQUAD_CPP
#define __HBIQUAD_CPP

#include "hbiquad.h"

template <class T>
HBiQuad<T>::HBiQuad(float fCutOff, float rate, float quality, float gain)
{
    float omegaCutOff = 2 * M_PI * (fCutOff / rate);

    // Basic intermediate values
    omegaC = cos(omegaCutOff);
    omegaS = sin(omegaCutOff);
    alpha = omegaS / (2.0f * quality);

    // Only used for shelving and peaking filters
    A = sqrt(pow(10, (gain / 20)));
    beta = sqrt(A) / quality;
}

template <class T>
float* HBiQuad<T>::Normalize()
{
    float a0 = biquadParameters[0];

    if( a0 != 1 )
    {
        normalizedBiquadParameters[0] = biquadParameters[1] / a0;
        normalizedBiquadParameters[1] = biquadParameters[2] / a0;
        normalizedBiquadParameters[2] = biquadParameters[3] / a0;
        normalizedBiquadParameters[3] = biquadParameters[4] / a0;
        normalizedBiquadParameters[4] = biquadParameters[5] / a0;
    }
    else
    {
        normalizedBiquadParameters[0] = biquadParameters[1];
        normalizedBiquadParameters[1] = biquadParameters[2];
        normalizedBiquadParameters[2] = biquadParameters[3];
        normalizedBiquadParameters[3] = biquadParameters[4];
        normalizedBiquadParameters[4] = biquadParameters[5];
    }

    return normalizedBiquadParameters;
}

template <class T>
float* HBiQuad<T>::Calculate()
{
    Calculate(omegaC, omegaS, alpha, A, beta, &biquadParameters[0], &biquadParameters[1], &biquadParameters[2], &biquadParameters[3], &biquadParameters[4], &biquadParameters[5]);
    return Normalize();
}

/********************************************************************
Explicit instantiation
********************************************************************/

// Initialize
template
HBiQuad<int8_t>::HBiQuad(float fCutOff, float rate, float quality, float gain);

template
HBiQuad<uint8_t>::HBiQuad(float fCutOff, float rate, float quality, float gain);

template
HBiQuad<int16_t>::HBiQuad(float fCutOff, float rate, float quality, float gain);

template
HBiQuad<int32_t>::HBiQuad(float fCutOff, float rate, float quality, float gain);

// Normalize
template
float* HBiQuad<int8_t>::Normalize();

template
float* HBiQuad<uint8_t>::Normalize();

template
float* HBiQuad<int16_t>::Normalize();

template
float* HBiQuad<int32_t>::Normalize();

// Calculate
template
float* HBiQuad<int8_t>::Calculate();

template
float* HBiQuad<uint8_t>::Calculate();

template
float* HBiQuad<int16_t>::Calculate();

template
float* HBiQuad<int32_t>::Calculate();

#endif