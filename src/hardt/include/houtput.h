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

        void Ready(O value)
        {
            if( _valueReady != NULL )
            {
                _valueReady(value);
            }
        }

        void Ready()
        {
            if( _ready != NULL )
            {
                _ready();
            }
        }

        bool Start()
        {
            return _writer != NULL ?_writer->Start() : true;
        }

        bool Stop()
        {
            return _writer != NULL ?_writer->Stop() : true;
        }

    public:

        HOutput():
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HOutput(HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(std::function<void(O*, size_t)> ready):
            HChunkWriter<T>(),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HOutput(std::function<void(O*, size_t)> ready, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(std::function<void(O)> ready):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HOutput(std::function<void(O)> ready, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(std::function<void()> ready):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready),
            _writer(NULL)
        {
        }

        HOutput(std::function<void()> ready, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(HWriter<O>* writer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
        }

        HOutput(HWriter<O>* writer, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
            consumer->SetWriter(this);
        }

        HOutput(int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HOutput(int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(std::function<void(O*, size_t)> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HOutput(std::function<void(O*, size_t)> ready, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(std::function<void(O)> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HOutput(std::function<void(O)> ready, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
            consumer->SetWriter(this);
        }

        HOutput(std::function<void()> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready)
        {
        }

        HOutput(std::function<void()> ready, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready)
        {
            consumer->SetWriter(this);
        }

        HOutput(HWriter<O>* writer, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
        }

        HOutput(HWriter<O>* writer, int chunksize, HWriterConsumer<T>* consumer):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
            consumer->SetWriter(this);
        }

        int WriteChunk(T* src, size_t blocksize)
        {
            return Output(src, blocksize);
        }

        virtual int Output(T* src, size_t blocksize) = 0;

        void SetWriter(HWriter<O>* writer)
        {
            _writer = writer;
        }
};

#endif