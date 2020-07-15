#ifndef __HGOERTZELOUTPUT_H
#define __HGOERTZELOUTPUT_H

/**
    Holds the result of a single or more Goertzl spectrum
    calculations over a set of samples.
*/
struct HGoertzelResult
{
    /** Calculated magnitude of the selected frequency bin for a block of samples */
    int Magnitude;

    /** Calculated phase of the selected frequency bin for a block of samples */
    int Phase;
};

/**
    Calculate one or more Goertzl spectrum results over a set
    of samples.

    When the requested number of calculations has been performed,
    the result object is written to the provided HWriter
    (usualy a HCustomWriter)
*/
template <class T>
class HGoertzelOutput : public HOutput<T, HGoertzelResult>
{
    private:

        int _size;
        int _average;
        float _bin;
        HWindow<T>* _window;
        T* _buffer;

        int _count;
        HGoertzelResult _result;

        float omega;
        float sine;
        float cosine;
        float coeff;
        float rad2degr;

        std::complex<float> _tempResult;

        void Init();

    public:

        /** Construct a new HGoertzelOutput, with a given frequency bin, that writes to a writer */
        HGoertzelOutput(int size, int average, float bin, HWriter<HGoertzelResult>* writer, HWindow<T>* window);

        /** Construct a new HGoertzelOutput, with a given frequency bin, that registers with an upstream writer */
        HGoertzelOutput(int size, int average, float bin, HWriterConsumer<T>* consumer, HWindow<T>* window);

        /** Construct a new HGoertzelOutput, from the sample rate and a frequency, that writes to a writer */
        HGoertzelOutput(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelResult>* writer, HWindow<T>* window);

        /** Construct a new HGoertzelFilter, from the sample rate and a frequency, that registers with an upstream writer */
        HGoertzelOutput(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriterConsumer<T>* consumer, HWindow<T>* window);

        /** Default destructor */
        ~HGoertzelOutput()
        {
            delete _buffer;
        }

        /** Generate output from a block of samples */
        int Output(T* src, size_t size);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }
};

#endif
