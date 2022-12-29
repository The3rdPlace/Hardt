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

        /** Construct a new HVfo running at the given frequency and phase
            with the given amplitude */
        HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, T amplitude, float phase = 0);

        /** Set the frequency and phase of the vfo */
        void SetFrequency(int frequency, float phase = 0);

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif