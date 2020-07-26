#ifndef __HIQ2REALCONVERTER_H
#define __HIQ2REALCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert from realvalues samples to IQ samples */
template <class T>
class HIq2RealConverter: public HConverter<std::complex<double>, T> {

    private:

        int Convert(std::complex<double>* src, T* dest, size_t blocksize) {

            for(int i = 0; i < blocksize; i++ ) {
                dest[i] = std::abs(src[i]);
            }

            return blocksize;
        }

        void Init(size_t blocksize, H_SAMPLE_RATE rate) {}

    public:

        /** Create a new iq-2-real converter with a reader */
        HIq2RealConverter(HReader<std::complex<double>>* reader, H_SAMPLE_RATE rate, size_t blocksize):
                HConverter<std::complex<double>, T>(reader, blocksize) {
            Init(blocksize, rate);
        }

        /** Create a new iq-2-real converter with a writer */
        HIq2RealConverter(HWriter<T>* writer, H_SAMPLE_RATE rate, size_t blocksize):
                HConverter<std::complex<double>, T>(writer, blocksize) {
            Init(blocksize, rate);
        }

        /** Create a new iq-2real converter with a writerconsumer */
        HIq2RealConverter(HWriterConsumer<std::complex<double>>* consumer, H_SAMPLE_RATE rate, size_t blocksize):
                HConverter<std::complex<double>, T>(consumer, blocksize) {
            Init(blocksize, rate);
        }

        /** Destruct this iq-2-real converter instance */
        ~HIq2RealConverter() {}
};

#endif
