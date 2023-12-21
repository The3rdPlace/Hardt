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
        HSplitter(std::string id, HWriter<T>* writer1, HWriter<T>* writer2):
            HWriter<T>(id),
            HWriterConsumer<T>(id) {
            HLog("HSplitter(HWriter*, HWriter*)");
            _writers.push_back(writer1);
            _writers.push_back(writer2);
        }

        /** Construct a new HSplitter */
        HSplitter(std::string id, std::vector< HWriter<T>* > writers):
            HWriter<T>(id),
            HWriterConsumer<T>(id) {
            HLog("HSplitter( std::vector< HWriter* >)");
            for( typename std::vector< HWriter<T>* >::iterator it = writers.begin(); it != writers.end(); it++ )
            {
                _writers.push_back( (*it) );
            }
        }

        /** Construct a new HSplitter which will later receive its
            writers via the HWriterConsumer scheme */
        HSplitter(std::string id, HWriterConsumer<T>* consumer):
            HWriter<T>(id),
            HWriterConsumer<T>(id) {
            HLog("HSplitter(HWriterConsumer*)");
            consumer->SetWriter(this);
        }

        /** Write a block of samples */
        int WriteImpl(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start();

        /** Cleanup after last write */
        bool Stop();

        /** Implements HWriterConsumer::SetWriter. Register writers for the splitter */
        void SetWriter(HWriter<T>* writer);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            for( typename std::vector< HWriter<T>* >::iterator it = _writers.begin(); it != _writers.end(); it++ )
            {
                if( !(*it)->Command(command) )
                {
                    return false;
                }
            }
            return true;
        }
};

#endif