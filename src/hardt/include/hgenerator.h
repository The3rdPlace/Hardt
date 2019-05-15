#ifndef __HGENERATOR_H
#define __HGENERATOR_H

#include "hardt.h"
#include "hsoundcard.h"
#include "hobject.h"
#include "hreader.h"
#include <math.h>

template <class T>
class HGenerator : public HReader<T>
{
    private:

        T* _lot;
        int _it;
        int _samplesPerCycle;

    protected:

        HGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase = 0);
        ~HGenerator();

    public:

        int Read(T* dest, size_t blocksize);
        void Calculate(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase);

};

#endif
