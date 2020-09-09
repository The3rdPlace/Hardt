#ifndef __HFIRDECIMATOR_H
#define __HFIRDECIMATOR_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

#include <vector>

/** Implements a fir-decimator that can reduce the incomming samplerate by a given factor */
template <class T>
class HFirDecimator: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksize;

        int _factor;
        T* _buffer;
        size_t _length;
        bool _collect;

        HFir<T>* _fir;
        T* _filtered;

        void Init(float* coefficients, int points);

    public:

        /** Construct a new HFirDecimator that handle writers.

            Parameters:
              writer = The downstream writer
              factor = Decimation factor, 1 or larger
              coefficients = FIR coefficients
              points = Number of FIR coefficients
              blocksize = The expected input and output blocksize
              collect = Normally you want to use the same blocksize for alle writers
                        in a chain, but if you are going to use a decimator in a parallel
                        demultiplexed chain (processing IQ data), it is very important to
                        write chunks from both branches alternating. In that case, set
                        'collect=false'. A write of 1024 samples will then immediately
                        result in a write of 1024/factor samples to the next writer.
         */
        HFirDecimator(HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize, bool collect = true);

        /** Construct a new HFirDecimator that handle writer consumers.

            Parameters:
              consumer = The upstream consumer to receive this as a writer
              factor = Decimation factor, 1 or larger
              coefficients = FIR coefficients
              points = Number of FIR coefficients
              blocksize = The expected input and output blocksize
              collect = Normally you want to use the same blocksize for alle writers
                        in a chain, but if you are going to use a decimator in a parallel
                        demultiplexed chain (processing IQ data), it is very important to
                        write chunks from both branches alternating. In that case, set
                        'collect=false'. A write of 1024 samples will then immediately
                        result in a write of 1024/factor samples to the next writer.
         */
        HFirDecimator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize, bool collect = true);

        /** Construct a new HFirDecimator that handle readers.

              reader = The upstream reader
              factor = Decimation factor, 1 or larger
              coefficients = FIR coefficients
              points = Number of FIR coefficients
              blocksize = The expected input and output blocksize
              collect = Normally you want to use the same blocksize for alle readers
                        in a chain, but if you are going to use a decimator in a parallel
                        demultiplexed chain (processing IQ data), it is very important to
                        read chunks from both branches alternating. In that case, set
                        'collect=false'. Constructing with 'blocksize=1024' then a Read()
                        with 'blocksize=256' will then be expected.
         */
        HFirDecimator(HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize, bool collect = true);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /** Default destructor */
        ~HFirDecimator();

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Call Start() on up- or downstream components */
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

        /** Call Stop() on up- or downstream components */
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

        /** Execute or carry through a command */
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
