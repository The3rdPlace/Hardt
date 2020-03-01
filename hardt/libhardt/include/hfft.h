#ifndef __HFFT_H
#define __HFFT_H

#include <complex>

/**
    Holds the result of a number of dfft's
*/
struct HFftResults
{
    /** The calculated/measured spectrum */
    double* Spectrum;

    /** The complex spectrum, in case you need to do phase calculations */
    std::complex<double>* Result;

    /** Size of spectrum (bins) */
    int Size;
};

/**
    Calculate a number of dfft's, if applicable then average the spectrum over
    the number of dfft's, then write the calculated spectrum results to the
    given writer (usually a HCustomWRiter)
*/
template <class T>
class HFft : public HOutput<T, HFftResults>
{
    private:

        int _size;
        int _average;

        double* _spectrum;
        std::complex<double>* _result;

        int _count;
        HWindow<T>* _window;

        T* _buffer;

        void Init();

    public:

        /** Create a new HFft output that writes to a writer */
        HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window);

        /** Create a new HFft output that registers with an upstream writer */
        HFft(int size, int average, HWriterConsumer<T>* consumer, HWindow<T>* window);

        /** Default destructor */
        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        /** Process a block of samples */
        int Output(T* src, size_t size);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }
};

#endif
