#ifndef __HCOSINEGENERATOR_H
#define __HCOSINEGENERATOR_H

template <class T>
class HCosineGenerator : public HGenerator<T>
{
    public:

        HCosineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude);
};

#endif