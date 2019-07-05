#ifndef __HLOCALOSCILLATOR_H
#define __HLOCALOSCILLATOR_H

template <class T>
class HLocalOscillator : public HGenerator<T>
{
    public:

        HLocalOscillator(H_SAMPLE_RATE rate, int frequency);
};

#endif