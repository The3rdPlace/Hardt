#ifndef __HGENERATOR_H
#define __HGENERATOR_H

#include "hardt.h"
#include "hsoundcard.h"
#include <math.h>

template <class T>
class HGenerator
{
    private:

        T* _lot;
        int _q1, _q2, _q3, _q4;
        int _it;

    protected:

        void GetSamples(T* dest, size_t blocksize);
        void Calculate(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase);

    public:

        HGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase = 0);
        ~HGenerator();
};

#endif
