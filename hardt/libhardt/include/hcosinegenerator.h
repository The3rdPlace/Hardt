#ifndef __HCOSINEGENERATOR_H
#define __HCOSINEGENERATOR_H

/**
    Generate a single cosine with the given frequency and amplitude
    at the given sample rate.
*/
template <class T>
class HCosineGenerator : public HGenerator<T>
{
    public:

        /** Construct a new cosine generator */
        HCosineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude);
};

#endif