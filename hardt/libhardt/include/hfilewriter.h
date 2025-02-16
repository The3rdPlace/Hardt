#ifndef __HFILEWRITER_H
#define __HFILEWRITER_H

#include <fstream>

#include "hardt.h"
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
        bool _seq;
        int _seqCount;

    public:

        /** Construct a new HFileWriter with the given filename */
        HFileWriter(std::string id, const char* filename, bool sequence = false):
            HWriter<T>(id),
            _filename(std::string(filename)),
            _seq(sequence)
        {}

        /** Construct a new HFileWriter with the given filename */
        HFileWriter(std::string id, const std::string filename, bool sequence = false):
            HWriter<T>(id),
            _filename(filename),
            _seq(sequence) {}

        /** Construct a new HFileWriter with the given filename and register
            with the given consumer */
        HFileWriter(std::string id, const char* filename, HWriterConsumer<T>* consumer, bool sequence = false):
            HWriter<T>(id),
            _filename(std::string(filename)),
            _seq(sequence) {
            consumer->SetWriter(this);
        }

        /** Construct a new HFileWriter with the given filename and register
            with the given consumer */
        HFileWriter(std::string id, const std::string filename, HWriterConsumer<T>* consumer, bool sequence = false):
            HWriter<T>(id),
            _filename(filename),
            _seq(sequence){
            consumer->SetWriter(this);
        }

        /** Write to the file */
        virtual int WriteImpl(T* src, size_t blocksize)
        {
            _stream.write((char*) src, blocksize * sizeof(T));
            return blocksize;
        }

        /** Initialize before first write */
        bool Start()
        {
            HLog("Trying to open stream for %s", _filename.c_str());
            std::string fname;
            if( _seq ) {
                fname = std::to_string(_seqCount++) + "-" + _filename;
            } else {
                fname = _filename;
            }
            _stream.open(fname.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
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