#ifndef __HBUFFEREDWRITER_H
#define __HBUFFEREDWRITER_H

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

        int _blocksReserved;
        HWriter<T>* _writer;
        std::vector<T*> _buffer;
        int _blocks;

    public:

        /** Create a new buffered writer with a default reserved buffer size */
        HBufferedWriter():
            _blocksReserved(DEFAULT_BLOCKS_RESERVED),
            _blocks(DEFAULT_BLOCKS_RESERVED)
        {
            _buffer.reserve( _blocks * sizeof(T*) );
        }

        /** Create a new buffered writer with a specific reserved buffer size */
        HBufferedWriter(int reserved):
                _blocksReserved(reserved),
                _blocks(reserved)
        {
            _buffer.reserve( _blocks * sizeof(T*) );
        }

        /** Delete a buffered writer */
        ~HBufferedWriter()
        {

        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start()
        {
            return _writer->Start();
        }

        /** Clean after last write */
        bool Stop()
        {
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

        /** This function must be implemented if you inherit this interface */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

        /** Write all buffered blocks before returning */
        void Flush()
        {

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
};

#endif
