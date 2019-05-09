#ifndef __HGENERATOR_H
#define __HGENERATOR_H

#include "hsoundcard.h"

template <class T>
class HGenerator
{
    private:

        T* _lot;
        int _length;
        int next;

    protected:

        void GetSamples(T* dest, size_t blocksize);

    public:

        HGenerator(H_SAMPLE_RATE rate, size_t blocksize, int frequency, int numberOfFrequencies, int phase);
        ~HGenerator();
};

#endif
