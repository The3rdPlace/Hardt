#ifndef __HCOSINEGENERATOR_H
#define __HCOSINEGENERATOR_H

template <class T>
class HCosineGenerator : public HReader<T>, public HGenerator<T>
{
    public:

        HCosineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude);

        int Read(T* dest, size_t blocksize);
};

#endif