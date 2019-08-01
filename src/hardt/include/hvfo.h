#ifndef __HVFO_H
#define __HVFO_H

/**
    Generate a single sine with the given frequency and amplitude
    at the given sample rate.

    The frequency and phase can be changed by calling the SetFrequency()
    function. Samples are _not_ guaranteed to have a smooth transaction
    when changing frequency or phase.
*/
template <class T>
class HVfo : public HGenerator<T>
{
    private:

        H_SAMPLE_RATE _rate;
        T _amplitude;

    public:

        HVfo(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase = 0);

        void SetFrequency(int frequency, float phase = 0);
};

#endif