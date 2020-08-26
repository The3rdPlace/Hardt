#ifndef __HDECIMATEBYFOUR_H
#define __HDECIMATEBYFOUR_H

#include "hfilter.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hprobe.h"

/**
    Translating the input samples by four.

    This implements the common frequency translation method
    of translating by four, either cos(PI*n) or sin(PI*n) by
    multiplying with [1, 0, -1, 0, 1, .... ] or [0, -1, 0, 1, 0, .... ]

    The result is either the real or imaginary (I or Q) signal
    of a quadrature signal decimated by 4 and with the center frequency
    shifted to 0.

    Do note that the output is either the I or Q branch of an combined
    IQ signal. If you look at the branches in a realvalued fft, you
    will find that

    1) The input spectrum [0 - fs/4] will appear shifted up past fs/4
    2) The input spectrum [0 - fs/4] will also appear as shifted towards 0 AND inverted.

    Components above fs/4 will be shifted towards 0 not-inverted, but you will
    get an alias centered and to the left of fs/4.

    So..: The output MUST be used as a complex signal - or by carefully filtering it
    and applying some dsp magic dust to utilize the shifted spectrums.
*/
template <class T>
class HTranslateByFour : public HFilter<T>
{
    private:

        bool _isQ;

        static void DoFilter(T* src, T* dest, size_t blocksize, bool isQ);

    public:

        /** Construct a new HTranslateByFour object that writes to a writer */
        HTranslateByFour(HWriter<T>* writer, size_t blocksize, bool isQ = false, HProbe<T>* probe = NULL):
            HFilter<T>(writer, blocksize, probe),
            _isQ(isQ) {}

        /** Construct a new HTranslateByFour object that registers with an upstream writer */
        HTranslateByFour(HWriterConsumer<T>* consumer, size_t blocksize, bool isQ = false, HProbe<T>* probe = NULL):
            HFilter<T>(consumer, blocksize, probe),
            _isQ(isQ) {}

        /** Construct a new HTranslateByFour object that reads from a reader */
        HTranslateByFour(HReader<T>* reader, size_t blocksize, bool isQ = false, HProbe<T>* probe = NULL):
            HFilter<T>(reader, blocksize, probe),
            _isQ(isQ) {}

        /** Default destructor */
        ~HTranslateByFour() {}

        /** Run a block of samples through the translator */
        void Filter(T* src, T* dest, size_t blocksize) {
            HTranslateByFour<T>::DoFilter(src, dest, blocksize, _isQ);
        }

        /** Convert the realvalued input signal 'src' to a complex IQ signal */
        static void Translate(T* src, std::complex<T>* dest, size_t blocksize) {
            T* I;
            T* Q;

            I = new T[blocksize];
            Q = new T[blocksize];

            HTranslateByFour<T>::DoFilter(src, I, blocksize, false);
            HTranslateByFour<T>::DoFilter(src, Q, blocksize, true);

            for( int i = 0; i < blocksize; i++ ) {
                dest[i] = std::complex<T>(I[i], Q[i]);
            }

            delete I;
            delete Q;
        }

        /** Convert the realvalued input signal 'src' to a complex IQ signal */
        static void Translate(T* src, std::complex<double>* dest, size_t blocksize) {
            T* I;
            T* Q;

            I = new T[blocksize];
            Q = new T[blocksize];

            HTranslateByFour<T>::DoFilter(src, I, blocksize, false);
            HTranslateByFour<T>::DoFilter(src, Q, blocksize, true);

            for( int i = 0; i < blocksize; i++ ) {
                dest[i] = std::complex<double>((double) I[i], (double) Q[i]);
            }
        }
};

#endif