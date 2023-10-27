#ifndef __HIQADDORSUBTRACTCONVERTER_H
#define __HIQADDORSUBTRACTCONVERTER_H

#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"
#include "hconverter.h"

/** Convert from IQ to I+J or I-J.
    IQ samples is stored intermixed, with the I (real) samples first, then the Q (imaginary) sample.

    Beware that this conversion changes the number of blocks read or written to or from the
    previous or next component.

    If you Read() 1024 samples, then 2*1024=2048 samples will be read from the previous reader.
    If you Write() 1024 samples, then 1024/2=512 samples will be written to the next writer.
*/
template <class T>
class HIqAddOrSubtractConverter: public HConverter<T, T> {

    private:

        HProbe<T>* _probe;

        bool _subtract;

        /**
         * Convert the input samples to the output format
         *
         * @param src Buffer containing the input samples
         * @param dest Buffer for the converted output samples
         * @param blocksize Number of samples to convert
         * @return Number of samples converted
         */
        int Convert(T* src, T* dest, size_t blocksize) {

            if( _subtract ) {
                for( int i = 0; i < blocksize; i += 2) {
                    dest[i / 2] = src[i] - src[i + 1];
                }
            } else {
                for( int i = 0; i < blocksize; i += 2) {
                    dest[i / 2] = src[i] + src[i + 1];
                }
            }

            return blocksize / 2;
        }

        /**
         * Initialize
         *
         * @param blocksize Number of samples per conversion
         */
        void Init(size_t blocksize) {}

    public:

        /**
         * Create a new converter with a reader
         *
         * BEWARE that reading 'blocksize samples will read
         * 'blocksize' samples from the previous reader and
         *
         * @param id Element identifier
         * @param reader Upstream reader
         * @param subtract Set to true to subtract instead of adding
         * @param blocksize Number of samples to read per read
         * @param probe Probe
         */
        HIqAddOrSubtractConverter(std::string id, HReader<T>* reader, bool subtract, size_t blocksize, HProbe<T>* probe = nullptr):
                HConverter<T, T>(id, reader, blocksize * 2, blocksize, probe),
                _subtract(subtract) {

            Init(blocksize * 2);
        }

        /**
         * Create a new converter with a writer
         *
         * BEWARE that writing 'blocksize' samples will write
         * 'blocksize/2' samples to the next writer
         *
         * @param id Element identifier
         * @param writer Downstream writer
         * @param subtract Set to true to subtract instead of adding
         * @param blocksize Number of samples to write per write
         * @param probe Probe
         */
        HIqAddOrSubtractConverter(std::string id, HWriter<T>* writer, bool subtract, size_t blocksize, HProbe<T>* probe = nullptr):
                HConverter<T, T>(id, writer, blocksize, blocksize / 2, probe),
                _subtract(subtract) {

            Init(blocksize);
        }

        /**
         * Create a new converter with a writerconsumer
         *
         * BEWARE that writing 'blocksize' samples will write
         * 'blocksize/2' samples to the next writer
         *
         * @param id Element identifier
         * @param consumer Upstream consumer
         * @param subtract Set to true to subtract instead of adding
         * @param blocksize Number of samples to write per write
         * @param probe Probe
        */
        HIqAddOrSubtractConverter(std::string id, HWriterConsumer<T>* consumer, bool subtract, size_t blocksize, HProbe<T>* probe = nullptr):
                HConverter<T, T>(id, consumer, blocksize, blocksize / 2, probe),
                _subtract(subtract) {

            Init(blocksize);
        }

        /** Destruct this converter instance */
        ~HIqAddOrSubtractConverter() {}
};

#endif
