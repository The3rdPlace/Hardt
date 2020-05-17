#ifndef __HSIGNALLEVEL_H
#define __HSIGNALLEVEL_H

#include "hwriter.h"
#include "hwriterconsumer.h"

/**
    The result of a signallevel calculation on
    one or more blocks of samples.

    Max contains the largest seen sample value and Avg contains a tracked
    average that slowly moves toward the latest max value.

    The level is calculated so that a maximum sample value contained in a block of samples
    the equals the maximum possible value for the selected sample format, will report as
    54 DB or S 9. Avg contains the average over 10 blocks and AvgDb, AvgS contains the
    corresponding average signal level.

    1 S degree equals 6 DB, so for 8 bit sample formats, the full range of db values and/or
    S degrees can not be obtained, the resolution is too low, so that you will experience sudden
    jumps in the calculated signal level. At 16 bit or higher the problem does not exists.

    If you want to calculate the level differently, you can do that using the Max value.
*/
struct HSignalLevelResult
{
    /** Highest sample value seen in the processed block(s) */
    int Max;

    /** Smallest sample value seen in the processed block(s) */
    int Min;

    /** Average value of highest sample value seen in the lsat 10 processed blocks */
    int Avg;

    /** The highest sample value, in DB */
    int Db;

    /** The average sample value over 10 blocks, in DB */
    int AvgDb;

    /** The highest sample value, in S degrees */
    int S;

    /** The average sample value over 10 blocks, in S degress */
    int AvgS;

    /** The sum of all samples values. This is a unqualified measurement, so
        the best use is to provide an instantaneous measurement of received
        signal values at a single point in time (and location). The value is
        not directly comparable with results removed in time or distance */
    double Sum;
};

/** Reports signallevel for a sampled signal */
template <class T>
class HSignalLevel : public HOutput<T, HSignalLevelResult>
{
    private:

        HSignalLevelResult _result;
        int _ref;
        int _scale;

        int _avgCount;
        T* _avg;
        double *_avgSum;
        int _avgPos;

        float _factor;

        void Init();

    public:

        /** Construct a new HSignalLevel object */
        HSignalLevel(HWriter<HSignalLevelResult>* writer, int average, int ref = 54, int scale = 1);

        /** Construct a new HSignalLevel object and register with the upstream writer by the
            HWriterConsumer scheme */
        HSignalLevel(HWriterConsumer<T>* consumer, int average, int ref = 54, int scale = 1);

        /** Destruct a HSignalLevel object */
        ~HSignalLevel();

        /** Process a block of samples */
        int Output(T* src, size_t blocksize);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }
};

#endif
