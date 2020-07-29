#ifndef __HMULTIPLEXED2COMPLEXCONVERTER_H
#define __HMULTIPLEXED2COMPLEXCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert from multiplexed realvalued samples to complex samples. */
template <class T>
class HMultiplexed2ComplexConverter: public HConverter<T, std::complex<T>> {

    private:

        int Convert(T* src, std::complex<T>* dest, size_t blocksize) {
            for( int i = 0; i < blocksize; i += 2 ) {
                dest[i / 2] = std::complex<T>(src[i], src[i + 1]);
            }
            return blocksize / 2;
        }

    public:

        /** Create a new multiplexed-2-complex converter with a reader */
        HMultiplexed2ComplexConverter(HReader<T>* reader, size_t blocksize):
            HConverter<T, std::complex<T>>(reader, blocksize, blocksize / 2) {}

        /** Create a new multiplexed-2-complex converter with a writer */
        HMultiplexed2ComplexConverter(HWriter<std::complex<T>>* writer, size_t blocksize):
            HConverter<T, std::complex<T>>(writer, blocksize, blocksize / 2) {}

        /** Create a new multiplexed-2-complex converter with a writerconsumer */
        HMultiplexed2ComplexConverter(HWriterConsumer<T>* consumer, size_t blocksize):
            HConverter<T, std::complex<T>>(consumer, blocksize, blocksize / 2) {}

        /** Destruct this multiplexed-2-complex converter instance */
        ~HMultiplexed2ComplexConverter() {}
};

#endif
