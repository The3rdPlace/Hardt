#ifndef __HBASEBAND_H
#define __HBASEBAND_H

#include "hfilter.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"
#include "hsoundcard.h"
#include "hfft.h"

#include <complex>

/**
    A filter that translates a segment in the spectrum down
    to baseband and filters away everything above the baseband segment
*/
template <class T>
class HBaseband : public HFilter<T>
{
    private:

        int _blocksize;

        int _center;
        int _width;
        int _rate;

        std::complex<double>* _translated;
        std::complex<double>* _spectrum;
        HFft<T>* _fft;

        double* _cos;
        double* _sin;

        void Init();
        void PreCalculate();

    public:

        /** Construct a new HBaseband filter that writes to a writer */
        HBaseband(HWriter<T>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HBaseband filter that registers with an upstream writer */
        HBaseband(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HBaseband filter that reads from a reader */
        HBaseband(HReader<T>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HBaseband();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set a new baseband segment */
        void SetSegment(int center, int width);
};

#endif