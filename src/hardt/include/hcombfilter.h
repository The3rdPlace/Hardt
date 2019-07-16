#ifndef __HCOMBFILTER_H
#define __HCOMBFILTER_H

template <class T>
class HCombFilter : public HFilter<T>
{
    private:

        enum COMB_FILTER_TYPE {FEED_BACK, FEED_FORWARD};

    private:

        int _length;
        T* _taps;
        COMB_FILTER_TYPE _type;
        float _alpha;


        void Init(H_SAMPLE_RATE rate, size_t blocksize, int  frequency);

        virtual T FeedBack(T* src)
        {
            return *src;
        }

    public:

        HCombFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, size_t blocksize);
        HCombFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, size_t blocksize);

        ~HCombFilter();

        void Filter(T* src, T* dest, size_t blocksize);
};

#endif