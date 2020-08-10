#ifndef __HKAISERBESSEL_CPP
#define __HKAISERBESSEL_CPP

#include "hkaiserbessel.h"

template <class T>
HKaiserBessel<T>::HKaiserBessel(int fa, int fb, int rate, int points, int att):
    _points(points),
    _fa(fa),
    _fb(fb),
    _rate(rate),
    _att(att) {

    // Sanity check
    if( points % 2 == 0 ) {
        throw new HFilterInitializationException("Number of points in a Kaiser-Bessel filter must be an uneven number");
    }
    if( fa < 0 || fb < 0 ) {
        throw new HFilterInitializationException("Fstart and Fstop can not be negative");
    }
    if( fa > fb ) {
        throw new HFilterInitializationException("Fstart must be smaller than Fstop");
    }
    if( fa == 0 ) {
        _fa = 1;
    }

    // Coefficients buffer
    _coefficients = new float[_points];
}

template <class T>
float HKaiserBessel<T>::ZeroOrderBessel(float x)
{
    float d = 0;
    float ds = 1;
    float s = 1;

    do
    {
        d += 2;
        ds *= x*x/(d*d);
        s += ds;
    }
    while (ds > s*1e-6);

    return s;
}

template <class T>
float* HKaiserBessel<T>::Calculate() {

    int Np = ((_points - 1) / 2);
    float* A = new float[Np + 1];

    // Calculate the impulse response of the ideal filter
    // (Split up the parts of the calculation to ensure proper casting)
    A[0] = 2 * ((float) _fb - (float) _fa) / (float) _rate;
    for( int j = 1; j <= Np; j++ )
    {
        float x = 2 * j * M_PI * ((float) _fb / (float) _rate);
        float y = 2 * j * M_PI * ((float) _fa / (float) _rate);
        float z = j * M_PI;

        A[j] = ( sin( x ) - sin( y ) ) / z;
    }

    // Calculate a Kaiser-Bessel window which must be applied to the ideal
    // impulse respon
    // se.
    float Alpha;
    if( _att < 21 )
    {
        Alpha = 0;
    }
    else if( _att > 50 )
    {
        Alpha = 0.1102 * (_att - 8.7);
    }
    else
    {
        Alpha = 0.5842 * pow((_att - 21), 0.4) + 0.07886 * (_att - 21);
    }

    // Apply the above window to the ideal impulse response
    // (coefficients is symmetrical around Np - the center frequency)
    float Inoalpha = ZeroOrderBessel(Alpha);
    for( int j = 0; j <= Np; j++ )
    {
        _coefficients[Np + j] = A[j] * ZeroOrderBessel( Alpha * sqrt(1 - (j*j/(Np*Np)) ) ) / Inoalpha;
    }

    // Cleanup, unused variable
    delete[] A;

    // Fill in the right-side coefficients since these are
    // equal to the left-side coefficients.
    for( int j=0; j < Np; j++ )
    {
        _coefficients[j] = _coefficients[_points - 1 - j];
    }

    // Let specific filter implementations carry out any needed modifications to the coefficients
    Modify(_coefficients, _points);

    // return the finished coefficients
    return _coefficients;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HKaiserBessel
template
HKaiserBessel<int8_t>::HKaiserBessel(int fa, int fb, int rate, int points, int att);

template
HKaiserBessel<uint8_t>::HKaiserBessel(int fa, int fb, int rate, int points, int att);

template
HKaiserBessel<int16_t>::HKaiserBessel(int fa, int fb, int rate, int points, int att);

template
HKaiserBessel<int32_t>::HKaiserBessel(int fa, int fb, int rate, int points, int att);

// Calculate
template
float* HKaiserBessel<int8_t>::Calculate();

template
float* HKaiserBessel<uint8_t>::Calculate();

template
float* HKaiserBessel<int16_t>::Calculate();

template
float* HKaiserBessel<int32_t>::Calculate();

//! @endcond
#endif