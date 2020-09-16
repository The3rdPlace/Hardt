#ifndef __HLOWPASSCOMBFILTER_H
#define __HLOWPASSCOMBFILTER_H

#include "hcombfilter.h"

/**
    Filter that removes or reduces mains hum by combining
    a comb filter with a lowpass filter in the feedback branch,
    which gradually reduces the filter effect. The result is a
    lowpass filter with extra notches at the mains frequency and
    harmonics. By giving the hum frequency and a cutoff frequency,
    you can adjust how aggressive the filter is and how far up
    the lowpass effect reaches.
*/
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

        /** Construct a new HHumFilter that writes to a writer */
        HHumFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize, HProbe<T>* probe = NULL):
            HCombFilter<T>(writer, rate, frequency, -0.5, blocksize, probe)
        {
            HLog("HHumFilter(HWriter*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1, probe);
            _buffer = new T[blocksize];
        }

        /** Construct a new HHumFilter that register with an upstream writer */
        HHumFilter(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize, HProbe<T>* probe = NULL):
            HCombFilter<T>(consumer, rate, frequency, -0.5, blocksize, probe)
        {
            HLog("HHumFilter(HWriter*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1, probe);
            _buffer = new T[blocksize];
        }

        /** Construct a new HHumFilter that reads from a reader */
        HHumFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize, HProbe<T>* probe = NULL):
            HCombFilter<T>(reader, rate, frequency, -0.5, blocksize, probe)
        {
            HLog("HHumFilter(HReader*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1, probe);
            _buffer = new T[blocksize];
        }

        /** Default destructor */
        ~HHumFilter()
        {
            delete  _feedbackFilter;
            delete _outputFilter;
            delete _buffer;
        }

        /** Run a block of samples through the filter */
        void Filter(T* src, T* dest, size_t blocksize)
        {
            HCombFilter<T>::Filter(src, _buffer, blocksize);
            _outputFilter->Filter(_buffer, dest, blocksize);
        }
};

#endif