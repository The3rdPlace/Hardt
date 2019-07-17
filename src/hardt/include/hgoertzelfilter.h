#ifndef __HGOERTZELFILTER_H
#define __HGOERTZELFILTER_H

#include <complex>

template <class T>
class HGoertzelFilter : public HOutput<T, int>
{
    private:

        int _size;
        int _average;
        int _bin;
        HWindow<T>* _window;
        T* _buffer;

        int _magnitude;

        int _count;

    public:

        HGoertzelFilter(int size, int average, int bin, HWriter<int>* writer, HWindow<T>* window);

        ~HGoertzelFilter()
        {
            delete _buffer;
        }

        int Output(T* src, size_t size);
};

#endif
