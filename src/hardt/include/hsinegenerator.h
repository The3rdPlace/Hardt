#ifndef __HSINEGENERATOR_H
#define __HSINEGENERATOR_H

/**
    Generate a single sine with the given frequency and amplitude
    at the given sample rate.
*/
template <class T>
class HSineGenerator : public HGenerator<T>
{
    public:

        HSineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude);
};

#endif