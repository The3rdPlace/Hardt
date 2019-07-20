#ifndef __HLOWPASSCOMBFILTER_H
#define __HLOWPASSCOMBFILTER_H

#include "hcombfilter.h"

template <class T>
class HHumFilter : public HCombFilter<T>
{
    private:

        HBiQuadFilter<HLowpassBiQuad<T>, T>* _feedbackFilter;
        HBiQuadFilter<HHighpassBiQuad<T>, T>* _outputFilter;
        T* _buffer;
        T dest[1];

        T FeedBack(T* src)
        {
            _feedbackFilter->Filter(src, dest, 1);
            return *dest;
        }

    public:

        HHumFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize):
            HCombFilter<T>(writer, rate, frequency, -0.5, blocksize)
        {
            HLog("HHumFilter(HWriter*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1);
            _buffer = new T[blocksize];
        }

        HHumFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize):
            HCombFilter<T>(reader, rate, frequency, -0.5, blocksize)
        {
            HLog("HHumFilter(HReader*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1);
            _buffer = new T[blocksize];
        }

        ~HHumFilter()
        {
            delete  _feedbackFilter;
            delete _outputFilter;
            delete _buffer;
        }

        void Filter(T* src, T* dest, size_t blocksize)
        {
            HCombFilter<T>::Filter(src, _buffer, blocksize);
            _outputFilter->Filter(_buffer, dest, blocksize);
        }
};

#endif