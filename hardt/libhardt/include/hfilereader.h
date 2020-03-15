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

    public:

        /** Construct a new HFileReader */
        HFileReader(const char* filename);

        /** Construct a new HFileReader */
        HFileReader(const std::string filename);

        /** Read a block of samples */
        virtual int Read(T* dest, size_t blocksize);

        /** Initialize before first read */
        bool Start();

        /** Cleanup after last read */
        bool Stop();

        /** Seek to this position in the file */
        void Seek(int bytes);

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif