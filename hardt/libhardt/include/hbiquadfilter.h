#ifndef __HBIQUADFILTER_H
#define __HBIQUADFILTER_H

/**
    A generic biquad filter. When created, give the
    wanted biquad type (HHighpassBiquad, HLowpassBiQuad, etc.) and the base
    datatype. Coefficients will be calculated by the specific biquad
    implementation and used to initialize a 2. order IIR (biquad) filter.
*/
template <class BIQUAD, class T>
class HBiQuadFilter: public HIirFilter<T>
{
    public:

        /** Construct a new HBiQuadFilter. The biquad type must be given together with the base
            datatype and the filter specifications */
        HBiQuadFilter(std::string id, HWriter<T>* writer, float fCutOff, float rate, float quality, float gain,  size_t blocksize, HProbe<T>* probe = NULL):
            HIirFilter<T>(id, writer, (BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5, blocksize, probe)
        {}

        /** Construct a new HBiQuadFilter. The biquad type must be given together with the base
            datatype and the filter specifications */
        HBiQuadFilter(std::string id, HWriterConsumer<T>* consumer, float fCutOff, float rate, float quality, float gain,  size_t blocksize, HProbe<T>* probe = NULL):
            HIirFilter<T>(id, consumer, (BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5, blocksize, probe)
        {}

        /** Construct a new HBiQuadFilter. The biquad type must be given together with the base
            datatype and the filter specifications */
        HBiQuadFilter(std::string id, HReader<T>* reader, float fCutOff, float rate, float quality, float gain,  size_t blocksize, HProbe<T>* probe = NULL):
            HIirFilter<T>(id, reader, (BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5, blocksize, probe)
        {}

        /** Default destructor */
        ~HBiQuadFilter()
        {}

        /** Called to run the IIR filter calculations on a block of samples */
        virtual void Filter(T* src, T* dest, size_t blocksize)
        {
            HIirFilter<T>::Filter(src, dest, blocksize);
        }

        /** Set filter coefficients by providing filter specifications */
        void SetCoefficients(float fCutOff, float rate, float quality, float gain,  size_t blocksize)
        {
            HIirFilter<int16_t>::SetCoefficients((BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5);
        }
};

#endif