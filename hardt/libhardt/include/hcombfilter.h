#ifndef __HCOMBFILTER_H
#define __HCOMBFILTER_H

/**
    Generic combfilter.

    Given the sampling rate, a frequency and the alpha value,
    creates a combfilter with notches or peaks at [freq., 2*freq, 3*freq, ... ]

    The alpha value controls if the filter creates notches or peaks.
*/
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

        /** Construct a new HCombFilter that writes to a writer */
        HCombFilter(std::string id, HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HCombFilter that registers with an upstream writer */
        HCombFilter(std::string id, HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, float alpha, size_t blocksize, HProbe<T>* probe = NULL);

        /** Constructs a new HCombFilter that reads from a reader */
        HCombFilter(std::string id, HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HCombFilter();

        /** Run a block of samples through the filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);
};

#endif