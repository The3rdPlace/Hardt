#ifndef __HREAL2IQCONVERTER_H
#define __HREAL2IQCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert from realvalued samples to IQ samples.
    IQ samples is stored intermixed, with the I (real) samples first, then the Q (imaginary) sample.

    Beware that this conversion changes the number of blocks read or written to or from the
    previous or next component.

    If you Read() 1024 samples, then 1024/2=512 samples will be read from the previous reader.
    If you Write() 1024 samples, then 1024*2=2048 samples will be written to the next writer.

    Typically, you would input the IQ data into a multiplexer, and then build two branches that
    uses the I and Q samples. These branches would then each have the same blocksize as the size
    you defined for the real-2-iq converter. */
template <class T>
class HReal2IqConverter: public HConverter<T, T> {

    private:

        HFft<T>* _fft;
        T* _output;

        int Convert(T* src, T* dest, size_t blocksize) {

            // Apply Hilbert transformation by taking the FFT, zero'ing the negative
            // frequencies and then taking the ifft of the result
            _fft->HILBERT(src, _output);

            // Interleave I and Q samples
            T* rIn = src;
            T* iIn = _output;
            T* out = dest;
            for( int i = 0; i < blocksize; i++ ) {
                *out = *(rIn++);
                out++;
                *out = *(iIn++);
                out++;
            }

            return blocksize * 2;
        }

        void Init(size_t blocksize) {
            _fft = new HFft<T>(blocksize);
            _output = new T[blocksize];
        }

    public:

        /** Create a new real-2-iq converter with a reader */
        HReal2IqConverter(HReader<T>* reader, size_t blocksize):
            HConverter<T, T>(reader, blocksize / 2, blocksize) {
            Init(blocksize);
        }

        /** Create a new real-2-iq converter with a writer */
        HReal2IqConverter(HWriter<T>* writer, size_t blocksize):
            HConverter<T, T>(writer, blocksize, blocksize * 2) {
            Init(blocksize);
        }

        /** Create a new real-2-iq converter with a writerconsumer */
        HReal2IqConverter(HWriterConsumer<T>* consumer, size_t blocksize):
            HConverter<T, T>(consumer, blocksize, blocksize * 2) {
            Init(blocksize);
        }

        /** Destruct this real-2-iq converter instance */
        ~HReal2IqConverter() {
            delete _fft;
            delete _output;
        }
};

#endif
