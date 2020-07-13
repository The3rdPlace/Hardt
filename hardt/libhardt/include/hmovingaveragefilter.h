#ifndef __HMOVINGAVERAGEFILTER_H
#define __HMOVINGAVERAGEFILTER_H

#include "hfilter.h"

/**
    A moving average filter
*/
template <class T>
class HMovingAverageFilter: public HFilter<T>
{
    private:

        int _m;
        double* _previous;
        
        void Init(size_t blocksize, int m) {
            if( _previous != nullptr ) {
                delete[] _previous;
            }
            _previous = new double[m - 1];
            memset((void*) _previous, 0, sizeof(T) * (m - 1));
        }

    public:

        /** Construct a new moving average filter with with a given M (number of samples to average over ) */
        HMovingAverageFilter(HWriter<T>* writer, int m,  size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(writer, blocksize, probe),
            _m(m),
            _previous(nullptr)
        {
            Init(blocksize, m);
        }

        /** Construct a new moving average filter with with a given M (number of samples to average over )  */
        HMovingAverageFilter(HWriterConsumer<T>* consumer, int m, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(consumer, blocksize, probe),
            _m(m),
            _previous(nullptr)
        {
            Init(blocksize, m);            
        }

        /** Construct a new moving average filter with with a given M (number of samples to average over )  */
        HMovingAverageFilter(HReader<T>* reader, int m, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(reader, blocksize, probe),
            _m(m),
            _previous(nullptr)
        {
            Init(blocksize, m);
        }

        /** Default destructor */
        ~HMovingAverageFilter()
        {}

        /** Called to run the filter calculations on a block of samples */
        void Filter(T* src, T* dest, size_t blocksize)
        {
            for(int i  = 0; i < blocksize; i++ ) {
                double sum = 0;

                for( int j = 0; j < _m; j++ ) {
                    if( i - _m + j + 1 < 0 ) {
                        sum += (double) _previous[j];
                    } else {
                        sum += (double) src[i - _m + j + 1];
                    }
                }
                
                sum /= _m;
                dest[i] = (T) sum;
            }
            
            for( int i = 0; i < _m - 1; i++ ) {
                _previous[i] = (double) src[blocksize - _m + i + 1];
            }
        }

        /** Set the moving average factor M (the number of adjacent samples being averaged) */
        void SetM(int m) {
            Init(HFilter<T>::_blocksize, m);
            _m = m;
        }
};

#endif