#ifndef __HCONVERTER_H
#define __HCONVERTER_H

#include <functional>

template <class T, class O>
class HConverter : public HWriter<T>
{
    private:

        std::function<void(O*, size_t)> _bufferReady;
        std::function<void(O)> _valueReady;
        std::function<void()> _ready;

    public:

        void Ready(O* buffer, size_t blocksize)
        {
            if( _bufferReady != NULL )
            {
                _bufferReady(buffer, blocksize);
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

    public:

        HConverter():
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(NULL)
        {
        }

        HConverter(std::function<void(O*, size_t)> ready):
            _bufferReady(ready),
            _valueReady(NULL),
            _ready(NULL)
        {
        }

        HConverter(std::function<void(O)> ready):
            _bufferReady(NULL),
            _valueReady(ready),
            _ready(NULL)
        {
        }

        HConverter(std::function<void()> ready):
            _bufferReady(NULL),
            _valueReady(NULL),
            _ready(ready)
        {
        }

        int Write(T* src, size_t blocksize)
        {
            return Convert(src, blocksize);
        }

        virtual int Convert(T* src, size_t blocksize) = 0;
};

#endif