#ifndef __HGOERTZELFILTER_H
#define __HGOERTZELFILTER_H

struct HGoertzelFilterResult
{
    int Magnitude;
    int Phase;
};

template <class T>
class HGoertzelFilter : public HOutput<T, HGoertzelFilterResult>
{
    private:

        int _size;
        int _average;
        float _bin;
        HWindow<T>* _window;
        T* _buffer;

        int _count;
        HGoertzelFilterResult _result;

        float omega;
        float sine;
        float cosine;
        float coeff;
        float rad2degr;

        std::complex<float> _tempResult;

        void Init();

    public:

        HGoertzelFilter(int size, int average, float bin, HWriter<HGoertzelFilterResult>* writer, HWindow<T>* window);
        HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelFilterResult>* writer, HWindow<T>* window);

        ~HGoertzelFilter()
        {
            delete _buffer;
        }

        int Output(T* src, size_t size);
};

#endif
