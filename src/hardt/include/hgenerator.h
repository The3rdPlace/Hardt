#ifndef __HGENERATOR_H
#define __HGENERATOR_H

#include "hardt.h"
#include "hsoundcard.h"
#include "hobject.h"
#include "hreader.h"
#include <math.h>
#include <limits>

template <class T>
class HGenerator : public HReader<T>
{
    private:

        T* _lot;
        int _lotSize;
        int _it;
        float _delta;
        H_SAMPLE_RATE _rate;

        float* _flot;

        float _zero;

        void Init(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase);

    protected:

        HGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase = 0);
        ~HGenerator();

    public:

        int Read(T* dest, size_t blocksize);
        void Calculate(int frequency, T amplitude, float phase);

};

#endif
