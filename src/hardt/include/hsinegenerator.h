#ifndef __HSINEGENERATOR_H
#define __HSINEGENERATOR_H

template <class T>
class HSineGenerator : public HReader<T>, public HGenerator<T>
{
    public:

        HSineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude);

        int Read(T* dest, size_t blocksize);
};

#endif