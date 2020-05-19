#ifndef __HBUFFEREDWRITER_H
#define __HBUFFEREDWRITER_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include "hwriter.h"
#include "hwriterconsumer.h"

#define DEFAULT_BLOCKS_RESERVED 1000

/**
    Buffer write operations if the downstream writer is slow or can pause randomly
*/
template <class T>
class HBufferedWriter : public HWriter<T>, public HWriterConsumer<T>
{
    private:

        size_t _blocksize;
        int _blocksReserved;
        HWriter<T>* _writer;
        std::vector<T*> _buffer;
        int _blocks;

        bool _enabled;

        std::thread* _drain;
        bool _isDraining;

        std::mutex _drainMutex;
        std::condition_variable _drainLock;

        std::mutex _readWriteMutex;

    public:

        /** Create a new buffered writer with a preset enabled/disabled state and with a specific reserved buffer size (in blocks) */
        HBufferedWriter(HWriterConsumer<T>* consumer, size_t blocksize, int reserved = DEFAULT_BLOCKS_RESERVED, bool enabled = true):
                _blocksize(blocksize),
                _blocksReserved(reserved),
                _blocks(reserved),
                _isDraining(false),
                _enabled(enabled)
        {
            Init();

            consumer->SetWriter(this->Writer());
        }

        /** Create a new buffered writer with a preset enabled/disabled state and with a specific reserved buffer size (in blocks) */
        HBufferedWriter(HWriter<T>* writer, size_t blocksize, int reserved = DEFAULT_BLOCKS_RESERVED, bool enabled = true):
                _blocksize(blocksize),
                _blocksReserved(reserved),
                _blocks(reserved),
                _isDraining(false),
                _enabled(enabled),
                _writer(writer)
        {
            Init();
        }

        /** Delete a buffered writer */
        ~HBufferedWriter()
        {
            StopDrain();

            HLog("Deleting remaining items from the buffer");
            while( _buffer.size() > 0 ) {
                delete _buffer.at(0);
                _buffer.erase( _buffer.begin(), _buffer.begin() + 1 );
            }
            HLog("Reducing the size of the buffer");
            _buffer.shrink_to_fit();
            HLog("Cleanup completed, ready to delete this object");
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start()
        {
            HLog("Start() called");
            if( _isDraining ) {
                throw new HWriterIOException("Drain thread is running. Writer must not be started again without being stopped first");
            }

            HLog("Starting downstream writer");
            bool started = _writer->Start();

            HLog("Starting drain thread");
            StartDrain();

            HLog("Drain thread is running");
            return started;
        }

        /** Clean after last write */
        bool Stop()
        {
            HLog("Stop() called");
            StopDrain();

            HLog("Drain thread is stopped, stopping downstream writer");
            return _writer->Stop();
        }

	    /** Receive, handle and propagate command

	        Returns:
		    True if the command was executed correctly
		    False otherwise. Errors is written to the error log
	    */
	    bool Command(HCommand* command)
        {
	        return _writer->Command(command);
        }

        /** Set the downstream writer */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

        /** Write all buffered blocks before returning */
        void Flush()
        {
            while( _isDraining && _buffer.size() > 0 ) {
                usleep(100);
            }
        }

        /** Get the number of blocks currently reserved in the buffer */
        int Reserved()
        {
            return _blocks;
        }

        /** Get the number of blocks currently stored in the buffer. The buffer is _not_ locked
            while fetching the number of blocks, so the count may have changed upon return */
        int Used()
        {
            return _buffer.size();
        }

        /** Returns true if buffering is enabled, otherwise false */
        bool Enabled()
        {
            return _enabled;
        }

    private:

        void Init() {
            _buffer.reserve( _blocks * sizeof(T*) );
        }

        void StartDrain() {

            // Start a new drain thread
            HLog("Starting drain thread");
            _isDraining = true;
            _drain = new std::thread( [this]()  {
                while( _isDraining ) {
                    Drain();
                }
                HLog("Drain thread exiting");
            } );
            HLog("Drain thread is running");
        }

        void StopDrain() {

            // Stop a running drain thread
            if( _isDraining && _drain != NULL ) {

                HLog("Signal halt to drain thread");
                _isDraining = false;

                HLog("Awake a waiting drain thread");
                _drainLock.notify_one();

                HLog("Joining drain thread");
                _drain->join();
                _drain = NULL;
                HLog("Drain thread joined");
            }
        }

        void Drain();
};

#endif
