#ifndef __HGAUSSIANFILTER_H
#define __HGAUSSIANFILTER_H

/**
    A filter that reduces gaussian noise
*/

#include "hfilter.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hprobe.h"

#include <complex>

template <class T>
class HGaussianFilter : public HFilter<T>
{
    private:

        size_t _blocksize;
        int _strength;
        int _n;
        int _blocks;
        HRectangularWindow<T>* _win;

        std::complex<double>* _response;
        HFft<T>* _fft;

        void Init() {
            if( _blocksize % _n != 0 ) {
                HError("%d is not dividable into %d chunks", _blocksize, _n);
                throw new HInitializationException("Blocksize can not be divided equally by the fft size");
            }

            _response = new std::complex<double>[_n];

            _win = new HRectangularWindow<T>();
            _fft =  new HFft<T>(_n, _win);

            _blocks = _blocksize / _n;
        }

    public:

        /** Construct a new HGaussianFilter object that writes to a writer
         *
         * @param id Element identifier
         * @param writer Downstream writer
         * @param blocksize Blocksize
         * @param strength Filter strength in db
         * @param n FFT size
         * @param probe A probe
         * */
        HGaussianFilter(std::string id, HWriter<T>* writer, size_t blocksize, int strength, int n, HProbe<T>* probe = NULL):
            HFilter<T>(id, writer, blocksize, probe),
            _blocksize(blocksize),
            _strength(strength),
            _n(n) {
            Init();
        }

        /** Construct a new HGaussianFilter object that registers with an upstream writer
         *
         * @param id Element identifier
         * @param consumer Upstream consumer
         * @param blocksize Blocksize
         * @param strength Filter strength in db
         * @param n FFT size
         * @param probe A probe
         * */
        HGaussianFilter(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, int strength, int n, HProbe<T>* probe = NULL):
            HFilter<T>(id, consumer, blocksize, probe),
            _blocksize(blocksize),
            _strength(strength),
            _n(n) {
            Init();
        }

        /** Construct a new HGaussianFilter object that reads from a reader
         *
         * @param id Element identifier
         * @param reader Upstream reader
         * @param blocksize Blocksize
         * @param strength Filter strength in db
         * @param n FFT size
         * @param probe A probe
         * */
        HGaussianFilter(std::string id, HReader<T>* reader, size_t blocksize, int strength, int n, HProbe<T>* probe = NULL):
            HFilter<T>(id, reader, blocksize, probe),
            _blocksize(blocksize),
            _strength(strength),
            _n(n) {
            Init();
        }

        /** Default destructor */
        ~HGaussianFilter() {
            delete _response;
            delete _fft;
            delete _win;
        }

        /** Run a block of samples through the filter */
        void Filter(T* src, T* dest, size_t blocksize);
};

#endif