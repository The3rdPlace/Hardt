#ifndef __HLOWPASSCOMBFILTER_H
#define __HLOWPASSCOMBFILTER_H

#include "hcombfilter.h"

template <class T>
class HLowpassCombFilter : public HCombFilter<T>
{
    private:

        HFilterBase<T>* _feedbackFilter;

        T FeedBack(T* src)
        {
            T dest[1];
            _feedbackFilter->Filter(src, dest, 1);
            return *dest;
        }

    public:

        HLowpassCombFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, int cutoffFrequency, float lowpassGain, size_t blocksize):
            HCombFilter<T>(writer, rate, frequency, alpha, blocksize)
        {
            HLog("HLowpassCombFilter(HWriter*, %d, %d, %f, %d, %f, %d)", rate, frequency, alpha, cutoffFrequency, lowpassGain, blocksize);
            _feedbackFilter = HBiQuadFactory< HLowpassBiQuad<T>, T >::Create((HReader<T>*) NULL, cutoffFrequency, rate, 0.7, lowpassGain, 1);
        }

        HLowpassCombFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, int cutoffFrequency, float lowpassGain, size_t blocksize):
            HCombFilter<T>(reader, rate, frequency, alpha, blocksize)
        {
            HLog("HLowpassCombFilter(HReader*, %d, %d, %f, %d, %f, %d)", rate, frequency, alpha, cutoffFrequency, lowpassGain, blocksize);
            _feedbackFilter = HBiQuadFactory< HLowpassBiQuad<T>, T >::Create((HReader<T>*) NULL, cutoffFrequency, rate, 0.7, lowpassGain, 1);
        }

        ~HLowpassCombFilter()
        {
            delete _feedbackFilter;
        }
};

#endif