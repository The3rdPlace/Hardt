#ifndef __HBIQUAD_H
#define __HBIQUAD_H

/*
    Biquads are defined by standard equations, the specific biquads based on this generic class
    has been defined using a nicely compiled list provided på Apogee Technology, inc.
    http://www.apogeebio.com/ddx/PDFs/AN-06.pdf
*/

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

        /** Construct a new HBiQuad */
        HBiQuad(float fCutOff, float rate, float quality, float gain);

    public:

        /** Calculate normalized coefficients */
        float* Calculate();
};

#endif