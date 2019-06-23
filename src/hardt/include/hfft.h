#ifndef __HFFT_H
#define __HFFT_H

#include <complex>

struct HFftResults
{
    double* Spectrum;
    std::complex<double>* Result;
    int Size;
};

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

    public:

        HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        int Output(T* src, size_t size);
};

#endif
