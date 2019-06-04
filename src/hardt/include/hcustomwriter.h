#ifndef __HCUSTOMWRITER_H
#define __HCUSTOMWRITER_H

template <class T>
class HCustomWriter : public HWriter<T>
{
    private:

        std::function<void(T*, size_t)> _customWriteFunc;

    public:

        HCustomWriter(std::function<void(T*, size_t)> customWriteFunc):
            _customWriteFunc(customWriteFunc)
        {}

        int Write(T* src, size_t blocksize)
        {
            _customWriteFunc(src, blocksize);
        }

        bool Start()
        {
            HWriter<T>::Start();
        }

        bool Stop()
        {
            HWriter<T>::Stop();
        }

        static HCustomWriter<T>* Create(void(*customWriteFunc)(T*, size_t blocksize))
        {
            std::function<void(T*, size_t)> func = customWriteFunc;

            return new HCustomWriter<T>(func);
        }

        template <typename F>
        static HCustomWriter<T>* Create(F* customWriterObject, void (F::*customWriteFunc)(T*, size_t blocksize))
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<void(T*, size_t)> func = std::bind( customWriteFunc, customWriterObject, _1, _2);;

            return new HCustomWriter<T>(func);
        }
};

#endif