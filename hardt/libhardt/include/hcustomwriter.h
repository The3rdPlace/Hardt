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

        /** Construct a HCustomWriter that wraps a function
            "int func(T* data, size_t size){}" by use of std::function */
        HCustomWriter(std::string id, std::function<int(T*, size_t)> customWriteFunc):
            HWriter<T>(id),
            _customWriteFunc(customWriteFunc)
        {}

        /** Construct a new HCustomWriter that wraps a function
            "int func(T* data, size_t size){}" by use of std::function
            and registers with an upstream writer */
        HCustomWriter(std::string id, std::function<int(T*, size_t)> customWriteFunc, HWriterConsumer<T>* consumer):
            HWriter<T>(id),
            _customWriteFunc(customWriteFunc)
        {
            consumer->SetWriter(this);
        }

        /** Write a block of data */
        int WriteImpl(T* src, size_t blocksize)
        {
            return _customWriteFunc(src, blocksize);
        }

        /** Initialize before first write */
        bool Start()
        {
            return true;
        }

        /** Cleanup after last write */
        bool Stop()
        {
            return true;
        }

        /** Factory function to create a new HCustomWriter by wrapping a static
            function "int func(T* data, size_t size){}" */
        static HCustomWriter<T>* Create(std::string id, int(*customWriteFunc)(T*, size_t blocksize))
        {
            std::function<int(T*, size_t)> func = customWriteFunc;

            return new HCustomWriter<T>(id, func);
        }

        /** Factory function to create a new HCustomWriter by wrapping a static
            function "int func(T* data, size_t size){}" and register with an upstream writer */
        static HCustomWriter<T>* Create(std::string id, int(*customWriteFunc)(T*, size_t blocksize), HWriterConsumer<T>* consumer)
        {
            std::function<int(T*, size_t)> func = customWriteFunc;

            return new HCustomWriter<T>(id, func, consumer);
        }

        /** Factory function to create a new HCustomWriter by wrapping a member function
            function "int F::func(T* data, size_t size){}" */
        template <typename F>
        static HCustomWriter<T>* Create(std::string id, F* customWriterObject, int (F::*customWriteFunc)(T*, size_t blocksize))
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<int(T*, size_t)> func = std::bind( customWriteFunc, customWriterObject, _1, _2);

            return new HCustomWriter<T>(id, func);
        }

        /** Factory function to create a new HCustomWriter by wrapping a member function
            function "int F::func(T* data, size_t size){}" and register with an upstream writer */
        template <typename F>
        static HCustomWriter<T>* Create(std::string id, F* customWriterObject, int (F::*customWriteFunc)(T*, size_t blocksize), HWriterConsumer<T>* consumer)
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<int(T*, size_t)> func = std::bind( customWriteFunc, customWriterObject, _1, _2);

            return new HCustomWriter<T>(id, func, consumer);
        }

        bool Command(HCommand* command) {
            // Terminates a command
            return true;
        }
};

#endif