#ifndef __HFFT_H
#define __HFFT_H

#include <complex>

/**
    Holds the result of a number of dfft's
*/
struct HFftResults
{
    double* Spectrum;
    std::complex<double>* Result;
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

        HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window);
        HFft(int size, int average, HWriterConsumer<T>* consumer, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        int Output(T* src, size_t size);
};

#endif
