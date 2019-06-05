#ifndef __HCUSTOMWRITER_H
#define __HCUSTOMWRITER_H

#include <functional>

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