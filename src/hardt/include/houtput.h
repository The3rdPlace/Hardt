#ifndef __HOUTPUT_H
#define __HOUTPUT_H

#include <functional>

/**
    The HOutput class is the base class for components that terminates a
    chain, such as the output of an DFFT calculation or a signal level indicator.
    An ouput may require a writer, but the data that the output writes is significant
    different, to the input samples written to the output. An fft calculation
    may require 4 blocks of 1024 samples, yet will only perform 1 write to a writer
    after having calculated the fft's and the averaged spectrum. The writer will then
    receive a write of length 1 and a pointer to the FFT result object.

    For clarity, the HSoundcardWriter is _not_  an HOutput since it actually outputs -
    allthough in a different physical form - the samples provided 1 to 1.

    An HOutput class can be created with any writer, but very few writers will accept
    any datatype other than sample values in an integer format, so the most common
    way to use an output type is to provide a HCustomWWriter object which then calls
    a locally defined handler.

    Internally, HOutput implements the HChunkWriter, so writes will be chunked to a size
    decided upon by the implementing class.
*/
template <class T, class O>
class HOutput : public HChunkWriter<T>, public HWriterConsumer<O>
{
    private:

        std::function<void(O*, size_t)> _bufferReady;
        std::function<void(O)> _valueReady;
        std::function<void()> _ready;
        HWriter<O>* _writer;

    public:

        /** A result is ready, buffer has the result and the length of the result is given by blocksize */
        void Ready(O* buffer, size_t blocksize)
        {
            if( _bufferReady != NULL )
            {
                _bufferReady(buffer, blocksize);
            }
            if( _writer != NULL )
            {
                _writer->Write(buffer, blocksize);
            }
        }

        /** A result is ready, the result has the value given by value */
        void Ready(O value)
        {
            if( _valueReady != NULL )
            {
                _valueReady(value);
            }
        }

        /** A result is ready, but no data is available (this behaves more like an event) */
        void Ready()
        {
            if( _ready != NULL )
            {
                _ready();
            }
        }

        /** Initialize before first write */
        bool Start()
        {
            return _writer != NULL ?_writer->Start() : true;
        }

        /** Cleanup after last write */
        bool Stop()
        {
            return _writer != NULL ?_writer->Stop() : true;
        }

    public:

        /** Construct a new HOutput */
        HOutput():
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput that registers with an upstream writer */
        HOutput(HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput that calls a function with a result pointer and the length of the result */
        HOutput(std::function<void(O*, size_t)> ready):
            HChunkWriter<T>(),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput that calls a function with a result pointer and the length of the result.
            The HOutput registers with the upstream writer */
        HOutput(std::function<void(O*, size_t)> ready, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput that calls a function with an int value */
        HOutput(std::function<void(O)> ready):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput that calls a function with an int value.
            The HOutput registers with the upstream writer */
        HOutput(std::function<void(O)> ready, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput that calls a function */
        HOutput(std::function<void()> ready):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput that calls a function.
            The HOutput registers with the upstream writer */
        HOutput(std::function<void()> ready, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput */
        HOutput(HWriter<O>* writer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
        }

        /** Construct a new HOutput */
        HOutput(HWriter<O>* writer, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput */
        HOutput(int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput */
        HOutput(int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput */
        HOutput(std::function<void(O*, size_t)> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput */
        HOutput(std::function<void(O*, size_t)> ready, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput */
        HOutput(std::function<void(O)> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
        }

        /** Construct a new HOutput */
        HOutput(std::function<void(O)> ready, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput */
        HOutput(std::function<void()> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready)
        {
        }

        /** Construct a new HOutput */
        HOutput(std::function<void()> ready, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready)
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HOutput */
        HOutput(HWriter<O>* writer, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
        }

        /** Construct a new HOutput */
        HOutput(HWriter<O>* writer, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
            consumer->SetWriter(this);
        }

        /** Write a chunk of samples */
        int WriteChunk(T* src, size_t blocksize)
        {
            return Output(src, blocksize);
        }

        /** Process this chunk of samples */
        virtual int Output(T* src, size_t blocksize) = 0;

        /** Implements HWriterConsumer::SetWriter */
        void SetWriter(HWriter<O>* writer)
        {
            _writer = writer;
        }
};

#endif