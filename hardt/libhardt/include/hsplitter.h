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
        {
            HLog("HSplitter(HWriter*, HWriter*)");
        }

        /** Construct a new HSplitter which will later receive its
            writers via the HWriterConsumer scheme */
        HSplitter(HWriterConsumer<T>* consumer):
            _writer1(NULL),
            _writer2(NULL)
        {
            HLog("HSplitter(HWriterConsumer*)");
            consumer->SetWriter(this);
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start()
        {
            HLog("Starting splitter writers");
            if( _writer1 != NULL )
            {
                HLog("Writer 1 is not null");
                if( !_writer1->Start() )
                {
                    HLog("Writer 1 failed to Start()");
                    return false;
                }
                HLog("Writer 1 started");
            }

            if( _writer2 != NULL )
            {
                HLog("Writer 2 is not null");
                if( !_writer2->Start() )
                {
                    HLog("Writer 2 failed to Start()");
                    return false;
                }
                HLog("Writer 2 started");
            }

            HLog("Done");
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
            HLog("HSplitter::SetWriter(HWriter*)");
            if( writer == _writer1 || writer == _writer2 ) {
                HLog("Writer is already registered, ignoring");
                return;
            }
            if( _writer1 == NULL )
            {
                HLog("Setting writer as writer 1");
                _writer1 = writer;
            }
            else if( _writer2 == NULL )
            {
                HLog("Setting writer as writer 2");
                _writer2 = writer;
            }
            else
            {
                throw new HInitializationException("HSplitter already has two writers");
            }
        }
};

#endif