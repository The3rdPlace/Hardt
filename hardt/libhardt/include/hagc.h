#ifndef __HAGC_H
#define __HAGC_H

#include "hgain.h"
#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"

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
        int _increment;
        bool _allowUnity;
        bool _enabled;

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
         * @param id Element identifier
         * @param writer Downstream writer
         * @param level Desired output level
         * @param average Number of blocks to average over when comparing to desired output level
         * @param blocksize Blocksize
         * @param increment When a larger or smaller gain is needed, gain is increased/decreased by this fraction of the full needed increment
         * @param allowUnity Allow use of unity gain (1), do not reduce gain further
         * 
         */
        HAgc(std::string id, HWriter<T>* writer, T level, int average, size_t blocksize, int increment = 4, bool allowUnity = false);

        /**
         * Construct a new HAgc object that registers with an upstream writer
         *
         * @param id Element identifier
         * @param consumer Upstream consumer
         * @param level Desired output level
         * @param average Number of blocks to average over when comparing to desired output level
         * @param blocksize Blocksize
         * @param increment When a larger or smaller gain is needed, gain is increased/decreased by this fraction of the full needed increment
         * @param allowUnity Allow use of unity gain (1), do not reduce gain further
         * 
         */
        HAgc(std::string id, HWriterConsumer<T>* consumer, T level, int average, size_t blocksize, int increment = 1, bool allowUnity = false);

        /**
         * Construct a new HAgc object that reads from a reader
         *
         * @param id Element identifier
         * @param reader Upstream reader
         * @param level Desired output level
         * @param average Number of blocks to average over when comparing to desired output level
         * @param blocksize Blocksize
         * @param increment When a larger or smaller gain is needed, gain is increased/decreased by this fraction of the full needed increment
         * @param allowUnity Allow use of unity gain (1), do not reduce gain further
         * 
         */
        HAgc(std::string id, HReader<T>* reader, T level, int average, size_t blocksize, int increment = 1, bool allowUnity = false);

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

        /** Enable or disable the automatic gain control */
        void SetEnabled(bool enabled) {
            _enabled = enabled;
        }

        /** Set the gain level. This will disable the automatic gain control */
        void SetGain(float gain) {
            _enabled = false;
            HGain<T>::SetGain(gain);
        }
};

#endif