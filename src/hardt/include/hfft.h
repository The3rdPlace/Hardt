#ifndef __HFFT_H
#define __HFFT_H

template <class T>
class HFft : public HConverter<T, long int>
{
    private:

        int _size;
        int _average;

        long int* _spectrum;

        int _count;
        HWindow<T>* _window;

        T* _buffer;

    public:

        HFft(int size, int average, HWriter<long int>* writer, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        int Convert(T* src, size_t size);
};

#endif
