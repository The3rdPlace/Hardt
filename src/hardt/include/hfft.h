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
        std::function<void(long int*, size_t)> _callback;

        HWindow<T>* _window;

        T* _buffer;

    public:

        HFft(int size, int average, std::function<void(long int*, size_t)> callback, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        static HFft* Create(int size, int average, void(*callback)(long int*, size_t length), HWindow<T>* window)
        {
            std::function<void(long int*, size_t)> func = callback;

            return new HFft<T>(size, average, func, window);
        }

        template <typename F>
        static HFft* Create(int size, int average, F* callbackObject, void (F::*callback)(long int*, size_t length), HWindow<T>* window)
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<void(long int*, size_t)> func = std::bind( callback, callbackObject, _1, _2);;

            return new HFft<T>(size, average, func, window);
            return NULL;
        }

        int Convert(T* src, size_t size);

        // Todo: Add convenience getters to return the frequency resolution etc.
};

#endif
