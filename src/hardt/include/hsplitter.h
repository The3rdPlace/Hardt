#ifndef __HSPLITTER_H
#define __HSPLITTER_H

/**
    Split a single chain of writers into two, so that samples can
    go, simultaneously to two chains
*/
template <class T>
class HSplitter : public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer1;
        HWriter<T>* _writer2;

    public:

        HSplitter(HWriter<T>* writer1, HWriter<T>* writer2):
            _writer1(writer1),
            _writer2(writer2)
        {}

        HSplitter(HWriterConsumer<T>* consumer):
            _writer1(NULL),
            _writer2(NULL)
        {
            consumer->SetWriter(this);
        }

        int Write(T* src, size_t blocksize);

        bool Start()
        {
            return _writer1->Start() && _writer2->Start();
        }

        bool Stop()
        {
            return _writer1->Stop() && _writer2->Stop();
        }

        void SetWriter(HWriter<T>* writer)
        {
            if( _writer1 == NULL )
            {
                _writer1 = writer;
            }
            else if( _writer2 == NULL )
            {
                _writer2 = writer;
            }
            else
            {
                throw new HInitializationException("HSplitter already has two writers");
            }
        }
};

#endif