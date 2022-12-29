#ifndef __HFILEREADER_H
#define __HFILEREADER_H

/**
    Read from a file
*/
template <class T>
class HFileReader : public HReader<T>
{
    private:

        std::ifstream _stream;
        const std::string _filename;
        HProbe<T>* _probe;

    public:

        /** Construct a new HFileReader */
        HFileReader(std::string id, const char* filename, HProbe<T>* probe = nullptr):
            HReader<T>(id),
            _filename(std::string(filename)),
            _probe(probe)
        {}

        /** Construct a new HFileReader */
        HFileReader(std::string id, const std::string filename, HProbe<T>* probe = nullptr):
            HReader<T>(id),
            _filename(filename),
            _probe(probe)
        {}

        /** Read a block of samples */
        virtual int Read(T* dest, size_t blocksize)
        {
            // Read next chunk
            _stream.read((char*) dest, blocksize * sizeof(T));

            // Probe output ?
            if( _probe != nullptr ) {
                _probe->Write(dest, blocksize * sizeof(T));
            }

            // Check for eof
            if( _stream.eof() )
            {
                HLog("At eof. Returning zero read");
                return 0;
            }

            return blocksize;
        }

        /** Initialize before first read */
        bool Start()
        {
            HLog("Trying to open stream for %s", _filename.c_str());
            _stream.open(_filename.c_str(), std::ios::binary);
            if( !_stream.is_open())
            {
                HError("Failed to open file %s", _filename.c_str());
                return false;
            }
            HLog("Stream is open");
            return true;
        }

        /** Cleanup after last read */
        bool Stop()
        {
            HLog("Closing stream");
            _stream.close();
            return true;
        }

        /** Seek to this position in the file */
        void Seek(int bytes)
        {
            _stream.seekg(bytes, std::ios::beg);
        }

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif