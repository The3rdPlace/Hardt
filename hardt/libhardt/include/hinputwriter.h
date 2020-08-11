#ifndef __HINPUTWRITER_H
#define __HINPUTWRITER_H

#include "hwriter.h"
#include "hwriterconsumer.h"

/**
    Write to a downstream writer without having any upstream chain components
    (direct input of samples to a chain of writers)
*/
template <class T>
class HInputWriter : public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        bool _autostart;
        bool _started;

    public:

        /** Construct a new HInputWriter with a downstream writer

            Arguments:
              writer = A downstream writer
              autostart = Set to true to make the input-writer call Start() on the downstream writer on first Write(..)
        */
        HInputWriter(HWriter<T>* writer, bool autostart = true):
            _writer(writer),
            _autostart(autostart),
            _started(false) {}

        /** Construct a new HInputWriter with an upstream writerconsumer

            Arguments:
              writer = A downstream writer
              autostart = Set to true to make the input-writer call Start() on the downstream writer on first Write(..)
        */
        HInputWriter(HWriterConsumer<T>* consumer, bool autostart = true):
            _writer(nullptr),
            _autostart(autostart),
            _started(false) {

            consumer->SetWriter(this->Writer());
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize) {
            if( _autostart && !_started ) {
                _started = Start();
            }
            return _writer->Write(src, blocksize);
        }

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /** Call Start() on downstream components */
        bool Start() {
            if( _writer != nullptr )
            {
                return _writer->Start();
            }
            return true;
        }

        /** Call Stop() on up- or downstream components */
        bool Stop() {
            if( _writer != nullptr )
            {
                return _writer->Stop();
            }
            return true;
        }

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            return _writer->Command(command);
        }
};

#endif