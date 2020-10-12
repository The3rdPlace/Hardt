#ifndef __HFILEWRITER_H
#define __HFILEWRITER_H

#include <stdio.h>
#include <fstream>

#include "hwriter.h"
#include "hwriterconsumer.h"

/**
    Write to a file
*/
template <class T>
class HFileWriter : public HWriter<T>
{
    private:

        std::ofstream _stream;
        const std::string _filename;

    public:

        /** Construct a new HFileWriter with the given filename */
        HFileWriter(const char* filename):
                _filename(std::string(filename))
        {}

        /** Construct a new HFileWriter with the given filename */
        HFileWriter(const std::string filename):
                _filename(filename)
        {}

        /** Construct a new HFileWriter with the given filename and register
            with the given consumer */
        HFileWriter(const char* filename, HWriterConsumer<T>* consumer):
                _filename(std::string(filename))
        {
            consumer->SetWriter(this);
        }

        /** Construct a new HFileWriter with the given filename and register
            with the given consumer */
        HFileWriter(const std::string filename, HWriterConsumer<T>* consumer):
                _filename(filename)
        {
            consumer->SetWriter(this);
        }

        /** Write to the file */
        virtual int Write(T* src, size_t blocksize)
        {
            _stream.write((char*) src, blocksize * sizeof(T));
            return blocksize;
        }

        /** Initialize before first write */
        bool Start()
        {
            HLog("Trying to open stream for %s", _filename.c_str());
            _stream.open(_filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
            if( !_stream.is_open())
            {
                HError("Failed to open file %s", _filename.c_str());
                return false;
            }
            HLog("Stream is open");
            return true;
        }

        /** Cleanup after last write */
        bool Stop()
        {
            HLog("Closing stream");
            _stream.close();
            return true;
        }

        /** Seek to this position in the file */
        void Seek(int bytes)
        {
            _stream.seekp(bytes, std::ios::beg);
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }
};

#endif