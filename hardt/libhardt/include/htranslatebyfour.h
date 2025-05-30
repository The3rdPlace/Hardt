#ifndef __HTRANSLATEBYFOUR_H
#define __HTRANSLATEBYFOUR_H

#include "hfilter.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"

/**
    Translating the input samples by four.

    This implements the common frequency translation method
    of translating by four, either cos(PI*n) or sin(PI*n) at fs/4 by
    multiplying with [1, 0, -1, 0, 1, .... ] or [0, -1, 0, 1, 0, .... ]

    The result is either the real or imaginary (I or Q) signal
    of a quadrature signal decimated by 4 and with the center frequency
    shifted to 0.

    You can think if this as the output being either the I or Q branch of an combined
    IQ signal. If you look at the branches in a realvalued fft, you
    will find that

    1) The input spectrum [0 - fs/4] will appear shifted up past fs/4
    2) The input spectrum [0 - fs/4] will also appear as shifted towards 0 AND inverted.

    Components above fs/4 will be shifted towards 0 not-inverted, but you will
    get an alias centered and to the left of fs/4.

    The flag 'isQ' simply signifies if you wants to multiply with cos or sin (isQ)
*/
template <class T>
class HTranslateByFour : public HFilter<T>
{
    private:

        bool _isQ;

        static void DoFilter(T* src, T* dest, size_t blocksize, bool isQ);

    public:

        /** Construct a new HTranslateByFour object that writes to a writer */
        HTranslateByFour(std::string id, HWriter<T>* writer, size_t blocksize, bool isQ = false):
            HFilter<T>(id, writer, blocksize),
            _isQ(isQ) {}

        /** Construct a new HTranslateByFour object that registers with an upstream writer */
        HTranslateByFour(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, bool isQ = false):
            HFilter<T>(id, consumer, blocksize),
            _isQ(isQ) {}

        /** Construct a new HTranslateByFour object that reads from a reader */
        HTranslateByFour(std::string id, HReader<T>* reader, size_t blocksize, bool isQ = false):
            HFilter<T>(id, reader, blocksize),
            _isQ(isQ) {}

        /** Default destructor */
        ~HTranslateByFour() = default;

        /** Run a block of samples through the translator */
        void Filter(T* src, T* dest, size_t blocksize) {
            HTranslateByFour<T>::DoFilter(src, dest, blocksize, _isQ);
        }

        /**
         * Change direction of the shift
         *
         * @param isQ shift direction, false=down, true=up
         */
        void SetIsQ(bool isQ) {
            _isQ = isQ;
        }

        /** Convert the realvalued input signal 'src' to a complex IQ signal */
        static void Translate(T* src, std::complex<T>* dest, size_t blocksize) {
            T* IChannel;
            T* QChannel;

            IChannel = new T[blocksize];
            QChannel = new T[blocksize];

            HTranslateByFour<T>::DoFilter(src, IChannel, blocksize, false);
            HTranslateByFour<T>::DoFilter(src, QChannel, blocksize, true);

            for( int i = 0; i < blocksize; i++ ) {
                dest[i] = std::complex<T>(IChannel[i], QChannel[i]);
            }

            delete IChannel;
            delete QChannel;
        }

        /** Convert the realvalued input signal 'src' to a complex IQ signal */
        static void Translate(T* src, std::complex<double>* dest, size_t blocksize) {
            T* IChannel;
            T* QChannel;

            IChannel = new T[blocksize];
            QChannel = new T[blocksize];

            HTranslateByFour<T>::DoFilter(src, IChannel, blocksize, false);
            HTranslateByFour<T>::DoFilter(src, QChannel, blocksize, true);

            for( int i = 0; i < blocksize; i++ ) {
                dest[i] = std::complex<double>((double) IChannel[i], (double) QChannel[i]);
            }
        }
};

#endif