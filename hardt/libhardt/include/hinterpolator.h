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

        HProbe<T>* _probe;

        int _factor;
        T* _inBuffer; 
        T* _outBuffer; 
        int _length;

        float * _coefficients;
        int _firLength;
        std::vector<HFir<T>*> _firs;

        void Init(float* coefficients = nullptr);

    protected:

        /**
         * Construct a new HInterpolator that handle writers.
         *
         * @param id Element identifier
         * @param writer The downstream writer
         * @param factor Interpolation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HInterpolator(std::string id, HWriter<T>* writer, int factor, size_t blocksize, HProbe<T>* probe = nullptr);

        /**
         * Construct a new HInterpolator that handle writer consumers.
         *
         * @param id Element identifier
         * @param consumer The upstream consumer to receive this as a writer
         * @param factor Interpolation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HInterpolator(std::string id, HWriterConsumer<T>* consumer, int factor, size_t blocksize, HProbe<T>* probe = nullptr);

        /**
         * Construct a new HInterpolator that handle readers.
         *
         * @param id Element identifier
         * @param reader The upstream reader
         * @param factor Interpolation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HInterpolator(std::string id, HReader<T>* reader, int factor, size_t blocksize, HProbe<T>* probe = nullptr);

    public:

        /**
         * Construct a new HInterpolator that handle writers.
         *
         * @param id Element identifier
         * @param writer The downstream writer
         * @param factor Interpolation factor, 1 or larger
         * @param coefficients FIR coefficients for the lowpass FIR filter that removes spectral copies of the baseband signal
         * @param length Number of FIR coefficients
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HInterpolator(std::string id, HWriter<T>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = nullptr);

        /**
         * Construct a new HInterpolator that handle writer consumers.
         *
         * @param id Element identifier
         * @param consumer The upstream consumer to receive this as a writer
         * @param factor Interpolation factor, 1 or larger
         * @param coefficients FIR coefficients for the lowpass FIR filter that removes spectral copies of the baseband signal
         * @param length Number of FIR coefficients
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HInterpolator(std::string id, HWriterConsumer<T>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = nullptr);

        /**
         * Construct a new HInterpolator that handle readers.
         *
         * @param id Element identifier
         * @param reader The upstream reader
         * @param factor Interpolation factor, 1 or larger
         * @param coefficients FIR coefficients for the lowpass FIR filter that removes spectral copies of the baseband signal
         * @param length Number of FIR coefficients
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HInterpolator(std::string id, HReader<T>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = nullptr);

        /**
         * Implements HWriterConsumer::SetWriter()
         *
         * @param writer Downstream writer
         * */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /**
         * Default destructor
         * */
        ~HInterpolator();

        /**
         * Write a block of samples
         *
         * @param src Source buffer
         * @param blocksize Number of samples in the buffer
         * */
        int Write(T* src, size_t blocksize);

        /**
         * Read a block of samples
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read into the buffer
         */
        int Read(T* dest, size_t blocksize);

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
         *
         * @param command The command
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
