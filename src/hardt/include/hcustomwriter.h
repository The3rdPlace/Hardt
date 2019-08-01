#ifndef __HCUSTOMWRITER_H
#define __HCUSTOMWRITER_H

#include <functional>

/**
    Provide a HWriter interface by wrapping a call to a function defined
    by your code.

    You must provide a std::function<T> object initialized with a function
    with the signature 'int func(T* src, size_t blocksize)' where T is
    the base integer type in use.

    The easiest way to create a customwriter is to use one of the factory methods
    Create(). They can either take a function pointer to a static (or global)
    function, or a function pointer to a member function in a given object.

    When used as a writer, each call to Write() will the call your local
    'function()' with the input sample buffer and the number of input samples.
    Your custom writer function must return the number of samples written
    (or consumed).
*/

template <class T>
class HCustomWriter : public HWriter<T>
{
    private:

        std::function<int(T*, size_t)> _customWriteFunc;

    public:

        HCustomWriter(std::function<int(T*, size_t)> customWriteFunc):
            _customWriteFunc(customWriteFunc)
        {}

        int Write(T* src, size_t blocksize)
        {
            return _customWriteFunc(src, blocksize);
        }

        bool Start()
        {
            return true;
        }

        bool Stop()
        {
            return true;
        }

        static HCustomWriter<T>* Create(int(*customWriteFunc)(T*, size_t blocksize))
        {
            std::function<int(T*, size_t)> func = customWriteFunc;

            return new HCustomWriter<T>(func);
        }

        template <typename F>
        static HCustomWriter<T>* Create(F* customWriterObject, int (F::*customWriteFunc)(T*, size_t blocksize))
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<int(T*, size_t)> func = std::bind( customWriteFunc, customWriterObject, _1, _2);;

            return new HCustomWriter<T>(func);
        }
};

#endif