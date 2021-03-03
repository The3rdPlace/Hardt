#ifndef __HAGC_H
#define __HAGC_H

#include "hgain.h"
#include "hardt.h"
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

        T _level;
        int _average;
        T *_averageBuffer;
        int _averagePtr;
        float _gain;
        int _hold;
        bool _allowUnity;

        void Init()
        {
            if( _average < 1 )
            {
                throw new HInvalidArgumentException("average count can not be less than 1");
            }
            _averageBuffer = new T[_average];
            memset((void*) _averageBuffer, 0, _average * sizeof(T));
            HLog("Allocated averaging buffer with %d bins", _average);
        }

    public:

        /**
         * Construct a new HAgc object that writes to a writer
         *
         * @param writer Downstream writer
         * @param level Desired output level
         * @param average Number of blocks to average over when comparing to desired output level
         * @param allowUnity Allow use of unity gain (1), do not reduce gain further
         * @param blocksize Blocksize
         * @param probe Probe
         */
        HAgc(HWriter<T>* writer, T level, int average, size_t blocksize, bool allowUnity = false, HProbe<T>* probe = NULL);

        /**
         * Construct a new HAgc object that registers with an upstream writer
         *
         * @param consumer Upstream consumer
         * @param level Desired output level
         * @param average Number of blocks to average over when comparing to desired output level
         * @param blocksize Blocksize
         * @param allowUnity Allow use of unity gain (1), do not reduce gain further
         * @param probe Probe
         */
        HAgc(HWriterConsumer<T>* consumer, T level, int average, size_t blocksize, bool allowUnity = false, HProbe<T>* probe = NULL);

        /**
         * Construct a new HAgc object that reads from a reader
         *
         * @param reader Upstream reader
         * @param level Desired output level
         * @param average Number of blocks to average over when comparing to desired output level
         * @param blocksize Blocksize
         * @param allowUnity Allow use of unity gain (1), do not reduce gain further
         * @param probe Probe
         */
        HAgc(HReader<T>* reader, T level, int average, size_t blocksize, bool allowUnity = false, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HAgc();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set bounds (lowest and highest value in the hysteresis interval) */
        void SetLevel(T level)
        {
            _level = level;
        }

        /** Set average (how many blocks is averaged before evaluating the gain again) */
        void SetAverage(int average)
        {
            _average = average;
        }
};

#endif