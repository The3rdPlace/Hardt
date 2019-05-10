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

    public:

        HGenerator(H_SAMPLE_RATE rate, int frequency, int phase);
        ~HGenerator();
};

#endif
