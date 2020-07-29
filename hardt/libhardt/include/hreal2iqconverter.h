#ifndef __HREAL2IQCONVERTER_H
#define __HREAL2IQCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"
#include "hhahnwindow.h"

/** Convert from realvalues samples to IQ samples.
    IQ samples is stored intermixed, with the I (real) samples first, then the Q (imaginary) sample.

    Be aware that this doubles to amount of data returned from a read, or written in a write. You
    need to handle the different blocksizes when you declare buffers and/or other components.

    Typically, you would input the IQ data into a multiplexer, and then build two branches that
    uses the I and Q samples. These branches would then each have the same blocksize as the size
    you defined for the real-2-iq converter. */
template <class T>
class HReal2IqConverter: public HConverter<T, T> {

    private:

        HHahnWindow<T> _window;
        HFft<T>* _fft;
        std::complex<double>* _fftOutput;
        T* _ifftOutput;

        int Convert(T* src, T* dest, size_t blocksize) {

            // Apply Hilbert transformation by taking the FFT, zero'ing the negative
            // frequencies and then taking the ifft of the result
            _fft->FFT(src, _fftOutput);
            for(int i = blocksize / 2; i < blocksize; i++ ) {
                _fftOutput = 0;
            }
            _fft->IFFT(_fftOutput, _ifftOutput);

            // Interleave I and Q samples
            T* rIn = src;
            T* iIn = _ifftOutput;
            T* out = dest;
            for( int i = 0; i < blocksize; i++ ) {
                *out = *(rIn++);
                out++;
                *out = *(iIn++);
                out++;
            }

            return blocksize * 2;
        }

        void Init(size_t blocksize, H_SAMPLE_RATE rate) {
            _fft = new HFft<T>(blocksize, &_window);
            _fftOutput = new std::complex<double>[blocksize];
            _ifftOutput = new T[blocksize];
        }

    public:

        /** Create a new real-2-iq converter with a reader */
        HReal2IqConverter(HReader<T>* reader, H_SAMPLE_RATE rate, size_t blocksize):
            HConverter<T, T>(reader, blocksize, blocksize * 2) {
            Init(blocksize, rate);
        }

        /** Create a new real-2-iq converter with a writer */
        HReal2IqConverter(HWriter<T>* writer, H_SAMPLE_RATE rate, size_t blocksize):
            HConverter<T, T>(writer, blocksize, blocksize * 2) {
            Init(blocksize, rate);
        }

        /** Create a new real-2-iq converter with a writerconsumer */
        HReal2IqConverter(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, size_t blocksize):
            HConverter<T, T>(consumer, blocksize, blocksize * 2) {
            Init(blocksize, rate);
        }

        /** Destruct this real-2-iq converter instance */
        ~HReal2IqConverter() {
            delete _fft;
            delete _fftOutput;
            delete _ifftOutput;
        }
};

#endif
