#ifndef __HFIRDECIMATOR_H
#define __HFIRDECIMATOR_H

#include "hdecimator.h"

/** Implements a decimator that can reduce the incomming samplerate by a given factor
    and at the same time filtering the input samples */
template <class T>
class HFirDecimator: public HDecimator<T>
{
    private:

        int _factor;

        void Init();

    public:

    /** Construct a new HDecimator that handle writers.

        Parameters:
          writer = The downstream writer
          factor = Decimation factor, 1 or larger
          blocksize = The expected input and output blocksize
     */
    HFirDecimator(HWriter<T>* writer, int factor, size_t blocksize);

    /** Construct a new HDecimator that handle writer consumers.

        Parameters:
          consumer = The upstream consumer to receive this as a writer
          factor = Decimation factor, 1 or larger
          blocksize = The expected input and output blocksize
     */
    HFirDecimator(HWriterConsumer<T>* consumer, int factor, size_t blocksize);

    /** Construct a new HDecimator that handle readers.

          reader = The upstream reader
          factor = Decimation factor, 1 or larger
          blocksize = The expected input and output blocksize
     */
    HFirDecimator(HReader<T>* reader, int factor, size_t blocksize);

    /** Default destructor */
    ~HFirDecimator();

    /** Write a block of samples */
    int Write(T* src, size_t blocksize);

    /** Read a block of samples */
    int Read(T* dest, size_t blocksize);
};

#endif
