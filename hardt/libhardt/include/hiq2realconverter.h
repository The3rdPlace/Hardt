#ifndef __HIQ2REALCONVERTER_H
#define __HIQ2REALCONVERTER_H

#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"
#include "hfft.h"

/** Convert from IQ to realvalued samples samples.
    IQ samples is stored intermixed, with the I (real) samples first, then the Q (imaginary) sample.

    Beware that this conversion changes the number of blocks read or written to or from the
    previous or next component.

    If you Read() 1024 samples, then 2*1024=2048 samples will be read from the previous reader.
    If you Write() 1024 samples, then 1024/2=512 samples will be written to the next writer.

    Typically, you will get IQ data from a quadratur-reading device such as an RTL-2832 dongle in IQ
    mode. Instead of demultiplexing the output into an I and Q branch and then do separate processing,
    if needed you can convert the multiplexed IQ stream to realvalued samples, and then work on the
    signal as if it was obtained as a realvalued signal.

    Warning: This conversion do take time, so it is best done with smaller blocksizes and/or samplerates.
    Also it will halve the max. frequency in your input due to removing the negative frequency space.
    So an RTL-2832 output taken at 1.152.000 samples per second, will then return signals in the range
    0-576.000 (relative to the center frequency which now becomes the leftmost frequency in the input spectrum.)
*/

template <class T>
class HIq2RealConverter: public HConverter<T, T> {

    private:

        HFft<T>* _fft;

        int Convert(T* src, T* dest, size_t blocksize) {

            _fft->IQ2REAL(src, dest);

            return blocksize / 2;
        }

        void Init(size_t blocksize) {

            // The IQ2REAL() function requires 'blocksize*2' input samples
            // to return 'blocksize' realvalued samples. So since the
            // input to this converter will have 'blocksize' samples, we
            // must create the fft object with 'blocksize/2' as blocksize
            _fft = new HFft<T>(blocksize / 2);
        }

    public:

        /** Create a new real-2-iq converter with a reader

            BEWARE that reading 'blocksize samples will read
            'blocksize' samples from the previous reader and
            return 'blocksize/2' samples
        */
        HIq2RealConverter(HReader<T>* reader, size_t blocksize):
                HConverter<T, T>(reader, blocksize * 2, blocksize) {
            Init(blocksize * 2);
        }

        /** Create a new real-2-iq converter with a writer

            BEWARE that writing 'blocksize' samples will write
            'blocksize/2' samples to the next writer
        */
        HIq2RealConverter(HWriter<T>* writer, size_t blocksize):
                HConverter<T, T>(writer, blocksize, blocksize / 2) {
            Init(blocksize);
        }

        /** Create a new real-2-iq converter with a writerconsumer

            BEWARE that writing 'blocksize' samples will write
            'blocksize/2' samples to the next writer
        */
        HIq2RealConverter(HWriterConsumer<T>* consumer, size_t blocksize):
                HConverter<T, T>(consumer, blocksize, blocksize / 2) {
            Init(blocksize);
        }

        /** Destruct this real-2-iq converter instance */
        ~HIq2RealConverter() {
            delete _fft;
        }
};

#endif
