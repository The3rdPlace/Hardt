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
        HHumFilter(std::string id, HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize):
            HCombFilter<T>(id, writer, rate, frequency, -0.5, blocksize)
        {
            HLog("HHumFilter(HWriter*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>(id, (HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>(id, (HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1);
            _buffer = new T[blocksize];
        }

        /** Construct a new HHumFilter that register with an upstream writer */
        HHumFilter(std::string id, HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize):
            HCombFilter<T>(id, consumer, rate, frequency, -0.5, blocksize)
        {
            HLog("HHumFilter(HWriter*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>(id, (HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>(id, (HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1);
            _buffer = new T[blocksize];
        }

        /** Construct a new HHumFilter that reads from a reader */
        HHumFilter(std::string id, HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int cutoffFrequency, size_t blocksize):
            HCombFilter<T>(id, reader, rate, frequency, -0.5, blocksize)
        {
            HLog("HHumFilter(HReader*, %d, %d, %d, %d)", rate, frequency, cutoffFrequency, blocksize);
            _feedbackFilter = new HBiQuadFilter<HLowpassBiQuad<T>, T>(id, (HReader<T>*) NULL, cutoffFrequency, rate, 0.5, 0, 1);
            _outputFilter = new HBiQuadFilter<HHighpassBiQuad<T>, T>(id, (HReader<T>*) NULL, cutoffFrequency, rate, 0.6, 0, 1);
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