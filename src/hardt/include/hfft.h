#ifndef __HFFT_H
#define __HFFT_H

struct HFftResults
{
    double* Spectrum;
    double* Phase;
    int Size;
};

template <class T>
class HFft : public HConverter<T, HFftResults>
{
    private:

        int _size;
        int _average;

        double* _spectrum;
        double* _phase;

        int _count;
        HWindow<T>* _window;

        T* _buffer;

    public:

        HFft(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _phase;
            delete _buffer;
        }

        int Convert(T* src, size_t size);
};

#endif
