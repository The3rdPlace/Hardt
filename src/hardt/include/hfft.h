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
        //std::function<void(long int*, size_t)> _callback;
        HCustomWriter<long int>* _writer;
        HWindow<T>* _window;

        T* _buffer;

    public:

        HFft(int size, int average, HCustomWriter<long int>* writer, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        int Convert(T* src, size_t size);

        // Todo: Add convenience getters to return the frequency resolution etc.
};

#endif
