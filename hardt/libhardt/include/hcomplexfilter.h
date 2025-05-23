#ifndef __HCOMPLEXFILTER_H
#define __HCOMPLEXFILTER_H

/**
    A filter performing filtering if an complex input with a (given/fixed)
    complex frequency response.
*/

#include "hfilter.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"

#include <complex>

template <class T>
class HComplexFilter : public HFilter<std::complex<T>>
{
    private:

        std::complex<T>* _response;
        size_t _blocksize;

        void Init(std::complex<T>* response) {
            _response = new std::complex<T>[_blocksize];
            memcpy((void*) _response, (void*) response, sizeof(std::complex<T>) * _blocksize);
        }

    public:

        /** Construct a new HComplexFilter object that writes to a writer */
        HComplexFilter(std::string id, HWriter<std::complex<T>>* writer, size_t blocksize, std::complex<T>* response):
            HFilter<std::complex<T>>(id, writer, blocksize),
            _blocksize(blocksize) {

            Init(response);
        }

        /** Construct a new HComplexFilter object that registers with an upstream writer */
        HComplexFilter(std::string id, HWriterConsumer<std::complex<T>>* consumer, size_t blocksize, std::complex<T>* response):
                HFilter<std::complex<T>>(id, consumer, blocksize),
                _blocksize(blocksize) {

            Init(response);
        }

        /** Construct a new HComplexFilter object that reads from a reader */
        HComplexFilter(std::string id, HReader<std::complex<T>>* reader, size_t blocksize, std::complex<T>* response):
                HFilter<std::complex<T>>(id, reader, blocksize),
                _blocksize(blocksize) {

            Init(response);
        }

        /** Default destructor */
        ~HComplexFilter() {
            delete _response;
        }

        /** Run a block of samples through the filter */
        void Filter(std::complex<T>* src, std::complex<T>* dest, size_t blocksize);
};

#endif