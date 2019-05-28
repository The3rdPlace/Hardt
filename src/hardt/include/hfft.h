#ifndef __HFFT_H
#define __HFFT_H

template <class T>
class HFft : public HConverter<T, long int>
{
    private:

        int _windowSize;
        int _bins;
        int _average;

        long int* _spectrum;

        int _count;
        std::function<void(long int*, size_t)> _callback;

    public:

        // Todo: Add parameter for specifying a window function (HWindow, aka. specialized filter)
        HFft(int windowSize, int bins, int average, std::function<void(long int*, size_t)> callback);

        ~HFft()
        {
            delete _spectrum;
        }

        static HFft* Create(int windowSize, int bins, int average, void(*callback)(long int*, size_t length))
        {
            std::function<void(long int*, size_t)> func = callback;

            return new HFft<T>(windowSize, bins, average, func);
        }

        template <typename F>
        static HFft* Create(int windowSize, int bins, int average, F* callbackObject, void (F::*callback)(long int*, size_t length))
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<void(long int*, size_t)> func = std::bind( callback, callbackObject, _1, _2);;

            return new HFft<T>(windowSize, bins, average, func);
            return NULL;
        }

        int Convert(T* src, size_t windowSize);

        // Todo: Add convenience getters to return the frequency resolution etc.
};

#endif
