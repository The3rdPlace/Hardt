#ifndef __HIQTRANSLATEBYFOUR_H
#define __HIQTRANSLATEBYFOUR_H

#include "hfilter.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hprobe.h"

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
class HIqTranslateByFour : public HFilter<T>
{
    private:

        bool _isQ;

        static void DoFilter(T* src, T* dest, size_t blocksize, bool isQ);

    public:

        /** Construct a new HIqTranslateByFour object that writes to a writer */
        HIqTranslateByFour(std::string id, HWriter<T>* writer, size_t blocksize, bool isQ = false, HProbe<T>* probe = NULL):
            HFilter<T>(id, writer, blocksize, probe),
            _isQ(isQ) {}

        /** Construct a new HIqTranslateByFour object that registers with an upstream writer */
        HIqTranslateByFour(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, bool isQ = false, HProbe<T>* probe = NULL):
            HFilter<T>(id, consumer, blocksize, probe),
            _isQ(isQ) {}

        /** Construct a new HIqTranslateByFour object that reads from a reader */
        HIqTranslateByFour(std::string id, HReader<T>* reader, size_t blocksize, bool isQ = false, HProbe<T>* probe = NULL):
            HFilter<T>(id, reader, blocksize, probe),
            _isQ(isQ) {}

        /** Default destructor */
        ~HIqTranslateByFour() {}

        /** Run a block of samples through the translator */
        void Filter(T* src, T* dest, size_t blocksize) {
            HIqTranslateByFour<T>::DoFilter(src, dest, blocksize, _isQ);
        }

        /**
         * Change direction of the shift
         *
         * @param isQ shift direction, false=down, true=up
         */
        void SetIsQ(bool isQ) {
            _isQ = isQ;
        }
};

#endif