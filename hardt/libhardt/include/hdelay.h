#ifndef __HDELAY_H
#define __HDELAY_H

#include "hfilter.h"
#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

/**
    A filter providing a delay of the input samples
*/
template <class T>
class HDelay : public HFilter<T>
{
    private:

        int _buffersize;
        int _inPos;
        int _outPos;
        T *_buffer;

        void Init(size_t blocksize, H_SAMPLE_RATE rate, int seconds)
        {
            // Calculate the number of blocks that must be buffered
            int blocksPerSeconds = ceil((float) rate / (float) blocksize);
            int blocks = (seconds * blocksPerSeconds);
            Init(blocksize, blocks);
        }

        void Init(size_t blocksize, int blocks)
        {
            _buffersize = blocks + 1;

            // Allocate and initialize buffer
            _buffer = new T[_buffersize * blocksize];
            memset((void*) _buffer, 0, _buffersize * blocksize * sizeof(T));

            // Set initial positions
            _inPos = _buffersize - 1;
            _outPos = 0;
        }

    public:

        /**
         * Construct a new HDelay object that writes to a writer
         *
         * @param writer Downstream writer
         * @param blocksize Blocksize
         * @param rate Sample rate used
         * @param seconds Seconds to delay the input
         * @param probe Probe
         */
        HDelay(HWriter<T>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<T>* probe = NULL);

        /**
         * Construct a new HDelay object that registers with an upstream writer
         *
         * @param consumer Upstream consumer
         * @param blocksize Blocksize
         * @param rate Sample rate used
         * @param seconds Seconds to delay the input
         * @param probe Probe
         */
        HDelay(HWriterConsumer<T>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<T>* probe = NULL);

        /**
         * Construct a new HDelay object that reads from a reader
         *
         * @param reader Upstream reader
         * @param blocksize Blocksize
         * @param rate Sample rate used
         * @param seconds Seconds to delay the input
         * @param probe Probe
         */
        HDelay(HReader<T>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds, HProbe<T>* probe = NULL);

        /**
         * Construct a new HDelay object that writes to a writer
         *
         * @param writer Downstream writer
         * @param blocksize Blocksize
         * @param blocks Number of blocks to delay
         * @param probe Probe
         */
        HDelay(HWriter<T>* writer, size_t blocksize, int blocks, HProbe<T>* probe = NULL);

        /**
         * Construct a new HDelay object that registers with an upstream writer
         *
         * @param consumer Upstream consumer
         * @param blocksize Blocksize
         * @param blocks Number of blocks to delay
         * @param probe Probe
         */
        HDelay(HWriterConsumer<T>* consumer, size_t blocksize, int blocks, HProbe<T>* probe = NULL);

        /**
         * Construct a new HDelay object that reads from a reader
         *
         * @param reader Upstream reader
         * @param blocksize Blocksize
         * @param blocks Number of blocks to delay
         * @param probe Probe
         */
        HDelay(HReader<T>* reader, size_t blocksize, int blocks, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HDelay();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);
};

#endif