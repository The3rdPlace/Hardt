#ifndef __HCOMBFILTER_H
#define __HCOMBFILTER_H

template <class T>
class HCombFilter : public HFilter<T>
{
    public:

        enum COMB_FILTER_TYPE {FEED_BACK, FEED_FORWARD};

    private:

        int _length;
        T* _taps;
        COMB_FILTER_TYPE _type;
        float _alpha;
        HFilterBase<T>* _feedbackFilter;

        void Init(H_SAMPLE_RATE rate, size_t blocksize, int  frequency);

    public:

        HCombFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, size_t blocksize):
            HCombFilter(writer, rate, frequency, alpha, type, NULL, blocksize)
        {}

        HCombFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, size_t blocksize):
            HCombFilter(reader, rate, frequency, alpha, type, NULL, blocksize)
        {}

        HCombFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<T>* feedbackFilter, size_t blocksize);
        HCombFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<T>* feedbackFilter, size_t blocksize);

        ~HCombFilter();

        void Filter(T* src, T* dest, size_t blocksize);
};

#endif