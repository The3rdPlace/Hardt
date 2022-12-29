#ifndef __HIQDECIMATOR_H
#define __HIQDECIMATOR_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

#include <vector>

/** Implements an IQ decimator that can reduce the incomming samplerate by a given factor */
template <class T>
class HIqDecimator: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksize;

        HProbe<T>* _probe;

        int _factor;
        T* _buffer;
        size_t _length;
        bool _collect;
        int _pos;

        void Init();

    public:

        /**
         * Construct a new HIqDecimator that handle writers.
         *
         * @param writer The downstream writer
         * @param factor Decimation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param collect Normally you want to use the same blocksize for alle writers
         *                in a chain, but if you are going to use a decimator in a parallel
         *                demultiplexed chain (processing IQ data), it is very important to
         *                write chunks from both branches alternating. In that case, set
         *                'collect=false'. A write of 1024 samples will then immediately
         *                result in a write of 1024/factor samples to the next writer.
         * @param probe Probe
         */
        HIqDecimator(std::string id, HWriter<T>* writer, int factor, size_t blocksize, bool collect = true, HProbe<T>* probe = nullptr);

        /**
         * Construct a new HIqDecimator that handle writer consumers.
         *
         * @param consumer The upstream consumer to receive this as a writer
         * @param  factor Decimation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param collect Normally you want to use the same blocksize for alle writers
         *                in a chain, but if you are going to use a decimator in a parallel
         *                demultiplexed chain (processing IQ data), it is very important to
         *                write chunks from both branches alternating. In that case, set
         *                'collect=false'. A write of 1024 samples will then immediately
         *                result in a write of 1024/factor samples to the next writer.
         * @param probe Probe
         */
        HIqDecimator(std::string id, HWriterConsumer<T>* consumer, int factor, size_t blocksize, bool collect = true, HProbe<T>* probe = nullptr);

        /**
         * Construct a new HIqDecimator that handle readers.
         *
         * @param reader The upstream reader
         * @param factor Decimation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param collect Normally you want to use the same blocksize for alle readers
         *                in a chain, but if you are going to use a decimator in a parallel
         *                demultiplexed chain (processing IQ data), it is very important to
         *                read chunks from both branches alternating. In that case, set
         *                'collect=false'. Constructing with 'blocksize=1024' then a Read()
         *                with 'blocksize=256' will then be expected.
         * @param probe Probe
         */
        HIqDecimator(std::string id, HReader<T>* reader, int factor, size_t blocksize, bool collect = true, HProbe<T>* probe = nullptr);

        /**
         * Implements HWriterConsumer::SetWriter()
         *
         * @param writer Downstream writer
         */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /**
         * Default destructor
         */
        ~HIqDecimator();

        /**
         * Write a block of samples
         *
         * @param src Source buffer
         * @param blocksize Number of samples to write
         */
        int Write(T* src, size_t blocksize);

        /**
         * Read a block of samples
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         */
        int Read(T* dest, size_t blocksize);

        /**
         * @return Call Start() on up- or downstream components
         */
        bool Start() {
            if( _writer != nullptr )
            {
                return _writer->Start();
            }
            if( _reader != nullptr )
            {
                return _reader->Start();
            }
            return true;
        }

        /**
         * @return Call Stop() on up- or downstream components
         */
        bool Stop() {
            if( _writer != nullptr )
            {
                return _writer->Stop();
            }
            if( _reader != nullptr )
            {
                return _reader->Stop();
            }
            return true;
        }

        /**
         * Execute or carry through a command
         */
        bool Command(HCommand* command) {
            if( _writer != nullptr )
            {
                return _writer->Command(command);
            }
            if( _reader != nullptr )
            {
                return _reader->Command(command);
            }
            return true;
        }

        /**
         * Set the decimation factor. Warning: This will reset the buffer position
         * and start collecting samples for a write from position 0 (zero)
         *
         * @param factor Decimation factor
         */
        void SetFactor(int factor) {
            _factor = factor;
            _length = 0;
        }
};

#endif
