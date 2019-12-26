#ifndef __HLOCALOSCILLATOR_H
#define __HLOCALOSCILLATOR_H

/**
    Signalgenerator that can be used where one needs an input
    for mixers or other types of injection. The signal has
    the amplitude [-1; 1] and thus mixes without producing unlinear
    terms.
*/
template <class T>
class HLocalOscillator : public HVfo<T>
{
    public:

        /** Construct a new HLocalOscillator with the given samplingrate and frequency */
        HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude = 10);
};

#endif