#ifndef __HGOERTZELFILTER_H
#define __HGOERTZELFILTER_H

#include <complex>

template <class T>
class HGoertzelFilter : public HOutput<T, int>
{
    private:

        float _size;
        int _average;
        float _bin;
        HWindow<T>* _window;
        T* _buffer;

        int _magnitude;
        int _count;

    public:

        HGoertzelFilter(int size, int average, float bin, HWriter<int>* writer, HWindow<T>* window);
        HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<int>* writer, HWindow<T>* window);

        ~HGoertzelFilter()
        {
            delete _buffer;
        }

        int Output(T* src, size_t size);
};

#endif
