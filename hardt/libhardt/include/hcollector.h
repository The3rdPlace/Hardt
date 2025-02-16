#ifndef __HCOLLECTOR_H
#define __HCOLLECTOR_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"

#include <vector>

/**
    Implements a component that collects blocks of samples until
    a requested size has been reached. All collected samples are then
    written or returned to the upstream reader.

    Usefull if you are using a converter that lowers the blocksize, but
    would prefer to use the same blocksize through the entire chain.
*/
template <class T>
class HCollector: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksizeIn;
        size_t _blocksizeOut;

        T* _buffer;
        int _collected;

    public:

        /**
         * Construct a new HCollector that handle writers
         *
         * @param id Element identifier
         * @param writer The downstream writer
         * @param blocksizeIn Input blocksize
         * @param blocksizeOut Output blocksize
         *
         */
        HCollector(std::string id, HWriter<T>* writer, size_t blocksizeIn, size_t blocksizeOut);

        /**
         * Construct a new HCollector that handle writer consumers
         *
         * @param id Element identifier
         * @param consumer The upstream consumer
         * @param blocksizeIn Input blocksize
         * @param blocksizeOut Output blocksize
         *
         */
        HCollector(std::string id, HWriterConsumer<T>* consumer, size_t blocksizeIn, size_t blocksizeOut);

        /**
         * Construct a new HCollector that handle readers
         *
         * @param id Element identifier
         * @param reader The upstream reader
         * @param blocksizeIn Input blocksize
         * @param blocksizeOut Output blocksize
         *
         */
        HCollector(std::string id, HReader<T>* reader, size_t blocksizeIn, size_t blocksizeOut);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer);

    public:

        /**
         * Default destructor
         */
        ~HCollector();

        /**
         * Write a block of samples
         *
         * @param src Source buffer
         * @param blocksize Number of samples in the buffer
         * @returns Number of samples written
         */
        int WriteImpl(T* src, size_t blocksize);

        /**
         * Read a block of samples
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         * @returns The number of samples read
         */
        int ReadImpl(T* dest, size_t blocksize);

        /**
         * Call Start() on up- or downstream component
         */
        bool Start();

        /**
         * Call Stop() on up- or downstream component
         */
        bool Stop();

        /**
         * Execute or carry through a command
         *
         * @param command The command
         */
        bool Command(HCommand* command) {
            if( _writer != nullptr )
            {
                if( !_writer->Command(command) )
                {
                    return false;
                }
            }
            if( _reader != nullptr )
            {
                if( !_reader->Command(command) )
                {
                    return false;
                }
            }
            return true;
        }
};

#endif
