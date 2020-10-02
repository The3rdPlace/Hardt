#ifndef __HAGC_H
#define __HAGC_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

/**
    A filter providing automatic gain (or attenuation)
*/
template <class T>
class HAgc : public HGain<T>
{
    private:

        T _lower;
        T _upper;
        float _overshoot;
        int _average;
        T *_averageBuffer;
        int _averagePtr;
        int _lock;
        int _locked;

        void Init()
        {
            if( _average < 1 )
            {
                throw new HInvalidArgumentException("average count can not be less than 1");
            }
            _averageBuffer = new T[_average];
            memset((void*) _averageBuffer, 0, _average * sizeof(T));
            HLog("Allocated averaging buffer with %d bins", _average);

            _overshoot = 1 - ((_upper - _lower) / _lower);
            HLog("Calculated overshoot = %f", _overshoot);
        }

    public:

        /** Construct a new HAgc object that writes to a writer */
        HAgc(HWriter<T>* writer, T lower, T upper, int average, int lock, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HAgc object that registers with an upstream writer */
        HAgc(HWriterConsumer<T>* consumer, T lower, T upper, int average, int lock, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HAgc object that reads from a reader */
        HAgc(HReader<T>* reader, T lower, T upper, int average, int lock, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HAgc();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set bounds (lowest and highest value in the hysteresis interval) */
        void SetBounds(T lower, T upper)
        {
            _lower = lower;
            _upper = upper;

            _overshoot = ((_upper - _lower) / _lower) + 1;
            HLog("Recalculated overshoot = %f", _overshoot);
        }

        /** Set average (how many blocks is averaged before evaluating the gain again) */
        void SetAverage(int average)
        {
            _average = average;
        }

        /** Set lock (the number of blocks to wait before evaluating the gain again) */
        void SetLock(int lock)
        {
            _lock = lock;
        }
};

#endif