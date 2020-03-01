#ifndef __HSPLITTER_H
#define __HSPLITTER_H

#include <vector>

/**
    Split a single chain of writers into two, so that samples can
    go, simultaneously to two chains
*/
template <class T>
class HSplitter : public HWriter<T>, public HWriterConsumer<T>
{
    private:

        std::vector< HWriter<T>* > _writers;

    public:

        /** Construct a new HSplitter */
        HSplitter(HWriter<T>* writer1, HWriter<T>* writer2)
        {
            HLog("HSplitter(HWriter*, HWriter*)");
            _writers.push_back(writer1);
            _writers.push_back(writer2);
        }

        /** Construct a new HSplitter */
        HSplitter( std::vector< HWriter<T>* > writers)
        {
            HLog("HSplitter( std::vector< HWriter* >)");
            for( typename std::vector< HWriter<T>* >::iterator it = writers.begin(); it != writers.end(); it++ )
            {
                _writers.push_back( (*it) );
            }
        }

        /** Construct a new HSplitter which will later receive its
            writers via the HWriterConsumer scheme */
        HSplitter(HWriterConsumer<T>* consumer)
        {
            HLog("HSplitter(HWriterConsumer*)");
            consumer->SetWriter(this);
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start();

        /** Cleanup after last write */
        bool Stop();

        /** Implements HWriterConsumer::SetWriter. Register writers for the splitter */
        void SetWriter(HWriter<T>* writer);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }
};

#endif