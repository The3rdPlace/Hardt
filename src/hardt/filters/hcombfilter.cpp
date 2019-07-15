#ifndef __HCOMBFILTER_CPP
#define __HCOMBFILTER_CPP

#include "hcombfilter.h"

template <class T>
HCombFilter<T>::HCombFilter(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<T>* feedbackFilter, size_t blocksize):
    HFilter<T>(writer, blocksize),
    _type(type),
    _alpha(alpha),
    _feedbackFilter(feedbackFilter)
{
    HLog("HCombFilter(HWriter*, %d, %d, %f, %d, %d)", rate, frequency, alpha, type, blocksize);
    Init(rate, blocksize, frequency);
}

template <class T>
HCombFilter<T>::HCombFilter(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<T>* feedbackFilter, size_t blocksize):
    HFilter<T>(reader, blocksize),
    _type(type),
    _alpha(alpha),
    _feedbackFilter(feedbackFilter)
{
    HLog("HCombFilter(HReader*, %d, %d, %f, %d, %d)", rate, frequency, alpha, type, blocksize);
    Init(rate, blocksize, frequency);
}

template <class T>
void HCombFilter<T>::Init(H_SAMPLE_RATE rate, size_t blocksize, int frequency)
{
    HLog("Init(%d, %d, %d)", rate, blocksize, frequency);

    /*
        sampleLength = 1 / rate;
        delay = 1 / frequency


        number_of_samples =    delay
                            ------------
                            sampleLength

        number of samples =           1
                                  ---------
                                  frequency
                            ----------------------
                                  sampleLength

        number of samples =           1
                                  ---------
                                  frequency
                            ----------------------
                                      1
                                    -----
                                     rate

        number of samples =     1 * rate      =     rate
                            ----------------    --------------
                              frequency * 1       frequency



        Highest possible frequency is defined by using just a simple sample for delay

            1 =   rate
                ---------
                frequency

            fMax = rate

        This corresponds to the samplerate, and by the nydquest theorem, we will only ever input frequencies
        at rate/2 Hz. So the  maximum allowed comb base frequency is half the sample rate


        Lowest possible frequency is achieved when using all samples from the block (as long as we dont
        handle multiple blocks by the delay buffer)

        blocksize  =    rate
                     -----------
                      frequency

        frequency =     rate
                     ----------
                      blocksize

    */

    // Check that the requested comb base frequency is sane
    float fMin = rate / blocksize;
    float fMax = rate / 2;
    if( frequency < fMin )
    {
        HError("Requested base frequency %d is lower than Fmin = %f", frequency, fMin);
        throw new HFilterInitializationException("Requested base frequency is lower than Fmin");
    }
    if( frequency > fMax )
    {
        HError("Requested base frequency %d is higher than Fmax = %f", frequency, fMax);
        throw new HFilterInitializationException("Requested base frequency is higher than Fmax");
    }

    // Required number of samples
    _length = rate / frequency;

    // Check the actual frequency using an integer number of delay samples
    float fReal = (float) rate / (float) _length;
    HLog("Requested base frequency = %d, actual base frequency = %f", frequency, fReal);

    // Allocate delay taps
    _taps = new T[_length];
    for( int i = 0; i < _length; i++ )
    {
        _taps[i] = 0;
    }
    HLog("Allocated and initialized delay buffer for %d taps", _length);

    // Make sure the alpha setting is sane
    if( _type == FEED_FORWARD && _alpha > 0 )
    {
        HError("Provided positive alpha value for feedforward type comb filter, converting to negative");
        _alpha = -1 * _alpha;
    }
    else if( _type == FEED_BACK && _alpha < 0 )
    {
        HError("Provided negative alpha value for feedback type comb filter, converting to positive");
        _alpha = -1 * _alpha;
    }
}

template <class T>
HCombFilter<T>::~HCombFilter()
{
    HLog("~HCombFilter()");
    delete[] _taps;
}

template <class T>
void HCombFilter<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Optimize by precalculating some variables
    T* feedback = (_type == FEED_FORWARD ? src : dest);
    size_t delayLength = (_length - 1) * sizeof(T);

    for( int i = 0; i < blocksize; i ++ )
    {
        dest[i] = src[i] + ( _alpha * (float) _taps[_length - 1]);
        memcpy((void*) &_taps[1], (void*) _taps, delayLength);

        if( _feedbackFilter == NULL )
        {
            _taps[0] = *(feedback + i);
        }
        else
        {
            _feedbackFilter->Filter(feedback + i, _taps, 1);
        }
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HCombFilter
template
HCombFilter<int8_t>::HCombFilter(HWriter<int8_t>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<int8_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<uint8_t>::HCombFilter(HWriter<uint8_t>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<uint8_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<int16_t>::HCombFilter(HWriter<int16_t>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<int16_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<int32_t>::HCombFilter(HWriter<int32_t>* writer, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<int32_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<int8_t>::HCombFilter(HReader<int8_t>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<int8_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<uint8_t>::HCombFilter(HReader<uint8_t>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<uint8_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<int16_t>::HCombFilter(HReader<int16_t>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<int16_t>* feedbackFilter, size_t blocksize);

template
HCombFilter<int32_t>::HCombFilter(HReader<int32_t>* reader, H_SAMPLE_RATE rate, int frequency, float alpha, COMB_FILTER_TYPE type, HFilterBase<int32_t>* feedbackFilter, size_t blocksize);

// Init
template
void HCombFilter<int8_t>::Init(H_SAMPLE_RATE rate, size_t blocksize, int frequency);

template
void HCombFilter<uint8_t>::Init(H_SAMPLE_RATE rate, size_t blocksize, int frequency);

template
void HCombFilter<int16_t>::Init(H_SAMPLE_RATE rate, size_t blocksize, int frequency);

template
void HCombFilter<int32_t>::Init(H_SAMPLE_RATE rate, size_t blocksize, int frequency);

// ~HCombFilter()
template
HCombFilter<int8_t>::~HCombFilter();

template
HCombFilter<uint8_t>::~HCombFilter();

template
HCombFilter<int16_t>::~HCombFilter();

template
HCombFilter<int32_t>::~HCombFilter();

// Filter
template
void HCombFilter<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HCombFilter<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HCombFilter<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HCombFilter<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

#endif