#ifndef __HCONVERTER_H
#define __HCONVERTER_H

#include <functional>

template <class T, class O>
class HConverter : public HChunkWriter<T>
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

        HConverter():
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HConverter(std::function<void(O*, size_t)> ready):
            HChunkWriter<T>(),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HConverter(std::function<void(O)> ready):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HConverter(std::function<void()> ready):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready),
            _writer(NULL)
        {
        }

        HConverter(HWriter<O>* writer):
            HChunkWriter<T>(),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
        }

        HConverter(int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HConverter(std::function<void(O*, size_t)> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HConverter(std::function<void(O)> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL),
            _writer(NULL)
        {
        }

        HConverter(std::function<void()> ready, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready)
        {
        }

        HConverter(HWriter<O>* writer, int chunksize):
            HChunkWriter<T>(chunksize),
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL),
            _writer(writer)
        {
        }

        int WriteChunk(T* src, size_t blocksize)
        {
            return Convert(src, blocksize);
        }

        virtual int Convert(T* src, size_t blocksize) = 0;
};

#endif