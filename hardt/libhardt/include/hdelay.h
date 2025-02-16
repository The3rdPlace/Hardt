#ifndef __HDELAY_H
#define __HDELAY_H

#include "hfilter.h"
#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"

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
         * @param id Element identifier
         * @param writer Downstream writer
         * @param blocksize Blocksize
         * @param rate Sample rate used
         * @param seconds Seconds to delay the input
         *
         */
        HDelay(std::string id, HWriter<T>* writer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

        /**
         * Construct a new HDelay object that registers with an upstream writer
         *
         * @param id Element identifier
         * @param consumer Upstream consumer
         * @param blocksize Blocksize
         * @param rate Sample rate used
         * @param seconds Seconds to delay the input
         *
         */
        HDelay(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

        /**
         * Construct a new HDelay object that reads from a reader
         *
         * @param id Element identifier
         * @param reader Upstream reader
         * @param blocksize Blocksize
         * @param rate Sample rate used
         * @param seconds Seconds to delay the input
         *
         */
        HDelay(std::string id, HReader<T>* reader, size_t blocksize, H_SAMPLE_RATE rate, int seconds);

        /**
         * Construct a new HDelay object that writes to a writer
         *
         * @param id Element identifier
         * @param writer Downstream writer
         * @param blocksize Blocksize
         * @param blocks Number of blocks to delay
         *
         */
        HDelay(std::string id, HWriter<T>* writer, size_t blocksize, int blocks);

        /**
         * Construct a new HDelay object that registers with an upstream writer
         *
         * @param id Element identifier
         * @param consumer Upstream consumer
         * @param blocksize Blocksize
         * @param blocks Number of blocks to delay
         *
         */
        HDelay(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, int blocks);

        /**
         * Construct a new HDelay object that reads from a reader
         *
         * @param id Element identifier
         * @param reader Upstream reader
         * @param blocksize Blocksize
         * @param blocks Number of blocks to delay
         *
         */
        HDelay(std::string id, HReader<T>* reader, size_t blocksize, int blocks);

        /** Default destructor */
        ~HDelay();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);
};

#endif