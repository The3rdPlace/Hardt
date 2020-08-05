#ifndef __HINTERPOLATOR_H
#define __HINTERPOLATOR_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

#include <vector>

/** Impements an interpolator that can increase the incomming samplerate by a given factor */
template <class T>
class HInterpolator: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksize;

        int _factor;
        T* _inBuffer; 
        T* _outBuffer; 
        int _length;

        float * _coefficients;
        int _firLength;
        std::vector<HFir<T>*> _firs;

        void Init(float* coefficients = nullptr);

    protected:

        /** Construct a new HInterpolator that handle writers.

            Parameters:
              writer = The downstream writer
              factor = Interpolation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HInterpolator(HWriter<T>* writer, int factor, size_t blocksize);

        /** Construct a new HInterpolator that handle writer consumers.

            Parameters:
              consumer = The upstream consumer to receive this as a writer
              factor = Interpolation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HInterpolator(HWriterConsumer<T>* consumer, int factor, size_t blocksize);

        /** Construct a new HInterpolator that handle readers.

              reader = The upstream reader
              factor = Interpolation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HInterpolator(HReader<T>* reader, int factor, size_t blocksize);

    public:

        /** Construct a new HInterpolator that handle writers.

            Parameters:
              writer = The downstream writer
              factor = Interpolation factor, 1 or larger
              coefficients = FIR coefficients for the lowpass FIR filter that removes spectral copies of the baseband signal
              length = Number of FIR coefficients
              blocksize = The expected input and output blocksize
         */
        HInterpolator(HWriter<T>* writer, int factor, float* coefficients, int length, size_t blocksize);

        /** Construct a new HInterpolator that handle writer consumers.

            Parameters:
              consumer = The upstream consumer to receive this as a writer
              factor = Interpolation factor, 1 or larger
              coefficients = FIR coefficients for the lowpass FIR filter that removes spectral copies of the baseband signal
              length = Number of FIR coefficients
              blocksize = The expected input and output blocksize
         */
        HInterpolator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int length, size_t blocksize);

        /** Construct a new HInterpolator that handle readers.

              reader = The upstream reader
              factor = Interpolation factor, 1 or larger
              coefficients = FIR coefficients for the lowpass FIR filter that removes spectral copies of the baseband signal
              length = Number of FIR coefficients
              blocksize = The expected input and output blocksize
         */
        HInterpolator(HReader<T>* reader, int factor, float* coefficients, int length, size_t blocksize);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /** Default destructor */
        ~HInterpolator();

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
