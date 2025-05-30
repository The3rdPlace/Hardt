#ifndef __HIQFIRDECIMATOR_H
#define __HIQFIRDECIMATOR_H

#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hfir.h"

#include <vector>

/** Implements a fir-decimator that can reduce the incomming samplerate by a given factor */
template <class T>
class HIqFirDecimator: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksize;

        int _factor;
        T* _buffer;
        T* _filtered;
        size_t _length;
        bool _collect;

        HFir<T>* _firI;
        HFir<T>* _firQ;


        void Init(float* coefficients, int points);

    public:

        /**
         * Construct a new HIqFirDecimator that handle writers.
         *
         * @param id Element identifier
         * @param writer The downstream writer
         * @param factor Decimation factor, 1 or larger
         * @param coefficients FIR coefficients
         * @param points Number of FIR coefficients
         * @param blocksize The expected input and output blocksize
         * @param collect Normally you want to use the same blocksize for alle writers
         *                in a chain, but if you are going to use a decimator in a parallel
         *                demultiplexed chain (processing IQ data), it is very important to
         *                write chunks from both branches alternating. In that case, set
         *                'collect=false'. A write of 1024 samples will then immediately
         *                result in a write of 1024/factor samples to the next writer.
         *
         */
        HIqFirDecimator(std::string id, HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect = true);

        /**
         * Construct a new HIqFirDecimator that handle writer consumers.
         *
         * @param id Element identifier
         * @param consumer Upstream consumer
         * @param factor Decimation factor, 1 or larger
         * @param coefficients FIR coefficients
         * @param points Number of FIR coefficients
         * @param blocksize The expected input and output blocksize
         * @param collect Normally you want to use the same blocksize for alle writers
         *                in a chain, but if you are going to use a decimator in a parallel
         *                demultiplexed chain (processing IQ data), it is very important to
         *                write chunks from both branches alternating. In that case, set
         *                'collect=false'. A write of 1024 samples will then immediately
         *                result in a write of 1024/factor samples to the next writer.
         *
         */
        HIqFirDecimator(std::string id, HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect = true);

        /**
         * Construct a new HIqFirDecimator that handle readers.
         *
         * @param id Element identifier
         * @param reader = The upstream reader
         * @param factor Decimation factor, 1 or larger
         * @param coefficients FIR coefficients
         * @param points Number of FIR coefficients
         * @param blocksize The expected input and output blocksize
         * @param collect Normally you want to use the same blocksize for alle readers
         *                in a chain, but if you are going to use a decimator in a parallel
         *                demultiplexed chain (processing IQ data), it is very important to
         *                write chunks from both branches alternating. In that case, set
         *                'collect=false'. A write of 1024 samples will then immediately
         *                result in a read of 1024 samples from the previous reader and
         *                return only 1024/factor samples.
         *
         */
        HIqFirDecimator(std::string id, HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect = true);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /**
         * Default destructor
         */
        ~HIqFirDecimator();

        /**
         * Write a block of samples
         *
         * @param src Source buffer
         * @param blocksize Number of samples to write
         */
        int WriteImpl(T* src, size_t blocksize);

        /**
         * Read a block of samples
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to write
         */
        int ReadImpl(T* dest, size_t blocksize);

        /**
         * Call Start() on up- or downstream components
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
         * Call Stop() on up- or downstream components
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
};

#endif
