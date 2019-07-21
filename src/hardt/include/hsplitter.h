#ifndef __HSPLITTER_H
#define __HSPLITTER_H

template <class T>
class HSplitter : public HWriter<T>
{
    private:

        HWriter<T>* _writer1;
        HWriter<T>* _writer2;

    public:

        HSplitter(HWriter<T>* writer1, HWriter<T>* writer2):
            _writer1(writer1),
            _writer2(writer2)
        {}

        int Write(T* src, size_t blocksize);

        bool Start()
        {
            return _writer1->Start() && _writer2->Start();
        }

        bool Stop()
        {
            return _writer1->Stop() && _writer2->Stop();
        }
};

#endif