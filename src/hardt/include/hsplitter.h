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

        /** Construct a new HSplitter */
        HSplitter(HWriter<T>* writer1, HWriter<T>* writer2):
            _writer1(writer1),
            _writer2(writer2)
        {}

        /** Construct a new HSplitter which will later receive its
            writers via the HWriterConsumer scheme */
        HSplitter(HWriterConsumer<T>* consumer):
            _writer1(NULL),
            _writer2(NULL)
        {
            consumer->SetWriter(this);
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start()
        {
            if( _writer1 != NULL )
            {
                if( !_writer1->Start() )
                {
                    return false;
                }
            }

            if( _writer2 != NULL )
            {
                if( !_writer2->Start() )
                {
                    return false;
                }
            }

            return true;
        }

        /** Cleanup after last write */
        bool Stop()
        {
            if( _writer1 != NULL )
            {
                if( !_writer1->Stop() )
                {
                    return false;
                }
            }

            if( _writer2 != NULL )
            {
                if( !_writer2->Stop() )
                {
                    return false;
                }
            }

            return true;
        }

        /** Implements HWriterConsumer::SetWriter. Register writers for the splitter */
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