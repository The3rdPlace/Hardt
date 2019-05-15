#ifndef __HVFO_H
#define __HVFO_H

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