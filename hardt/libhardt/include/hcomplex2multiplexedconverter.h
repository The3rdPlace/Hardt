#ifndef __HCOMPLEX2MULTIPLEXEDCONVERTER_H
#define __HCOMPLEX2MULTIPLEXEDCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert from complex samples to multiplexed realvalued samples */
template <class T>
class HComplex2MultiplexedConverter: public HConverter<std::complex<T>, T> {

    private:

        int Convert(std::complex<T>* src, T* dest, size_t blocksize) {

            for( int i = 0; i < blocksize * 2; i +=2 ) {
                dest[i] = src[i / 2].real();
                dest[i + 1] = src[i / 2].imag();
            }
            return blocksize * 2;
        }

    public:

        /** Create a new complex-2-multiplexed converter with a reader */
        HComplex2MultiplexedConverter(std::string id, HReader<std::complex<T>>* reader, size_t blocksize):
            HConverter<std::complex<T>, T>(id, reader, blocksize, blocksize * 2) {}

        /** Create a new complex-2-multiplexed converter with a writer */
        HComplex2MultiplexedConverter(std::string id, HWriter<T>* writer, size_t blocksize):
            HConverter<std::complex<T>, T>(id, writer, blocksize, blocksize * 2) {}

        /** Create a new complex-2-multiplexed converter with a writerconsumer */
        HComplex2MultiplexedConverter(std::string id, HWriterConsumer<std::complex<T>>* consumer, size_t blocksize):
            HConverter<std::complex<T>, T>(id, consumer, blocksize, blocksize * 2) {}

        /** Destruct this complex-2-multiplexed converter instance */
        ~HComplex2MultiplexedConverter() {}
};

#endif
