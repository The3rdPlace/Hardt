#ifndef __HHILBERTFILTER_H
#define __HHILBERTFILTER_H

/**
    A filter performing a Hilbert transformation on the input
    (returning the same signal delayed 90 degrees)
*/
template <class T>
class HHilbertFilter : public HFilter<T>
{
    private:

        HHahnWindow<T> _window;
        HFft<T>* _fft;
        std::complex<double>* _fftOutput;
        T* _ifftOutput;

        void Init(size_t blocksize) {
            _fft = new HFft<T>(blocksize, &_window);
            _fftOutput = new std::complex<double>[blocksize];
            _ifftOutput = new T[blocksize];
        }

    public:

        /** Construct a new HHilbertFilter object that writes to a writer */
        HHilbertFilter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(writer, blocksize, probe) {
            Init(blocksize);
        }

        /** Construct a new HHilbertFilter object that registers with an upstream writer */
        HHilbertFilter(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(consumer, blocksize, probe){
            Init(blocksize);
        }

        /** Construct a new HHilbertFilter object that reads from a reader */
        HHilbertFilter(HReader<T>* reader, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(reader, blocksize, probe){
            Init(blocksize);
        }

        /** Default destructor */
        ~HHilbertFilter() {
            delete _fft;
            delete _fftOutput;
            delete _ifftOutput;
        }

        /** Run a block of samples through the hilbert-filter */
        void Filter(T* src, T* dest, size_t blocksize) {
            _fft->HILBERT(src, dest);
        }
};

#endif