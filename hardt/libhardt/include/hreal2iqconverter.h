#ifndef __HREAL2IQCONVERTER_H
#define __HREAL2IQCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert from realvalues samples to IQ samples */
template <class T>
class HReal2IqConverter: public HConverter<T, std::complex<double>> {

    private:

        HLocalOscillator<double>* _sine;
        HLocalOscillator<double>* _cos;
        double* _sineData;
        double* _cosData;

        int Convert(T* src, std::complex<double>* dest, size_t blocksize) {

            _sine->Read(_sineData, blocksize);
            _cos->Read(_cosData, blocksize);

            for(int i = 0; i < blocksize; i++ ) {
                dest[i] = std::complex<double>(_sineData[i] * src[i], -1 * _cosData[i] * src[i]);
            }

            return blocksize;
        }

        void Init(size_t blocksize, H_SAMPLE_RATE rate) {
            _sine = new HLocalOscillator<double>(rate, rate/4, 1, 0 );
            _cos = new HLocalOscillator<double>(rate, rate/4, 1, -1 * M_PI / 2.0f);

            _sineData = new double[blocksize];
            _cosData = new double[blocksize];
        }

    public:

        /** Create a new real-2-iq converter with a reader */
        HReal2IqConverter(HReader<T>* reader, H_SAMPLE_RATE rate, size_t blocksize):
            HConverter<T, std::complex<double>>(reader, blocksize) {
            Init(blocksize, rate);
        }

        /** Create a new real-2-iq converter with a writer */
        HReal2IqConverter(HWriter<std::complex<double>>* writer, H_SAMPLE_RATE rate, size_t blocksize):
            HConverter<T, std::complex<double>>(writer, blocksize) {
            Init(blocksize, rate);
        }

        /** Create a new real-2-iq converter with a writerconsumer */
        HReal2IqConverter(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, size_t blocksize):
            HConverter<T, std::complex<double>>(consumer, blocksize) {
            Init(blocksize, rate);
        }

        /** Destruct this real-2-iq converter instance */
        ~HReal2IqConverter() {
            delete _sine;
            delete _cos;
            delete[] _sineData;
            delete[] _cosData;
        }
};

#endif
