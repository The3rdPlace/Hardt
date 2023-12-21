#ifndef __HPROCESSOR_H
#define __HPROCESSOR_H

#include "hwriter.h"
#include "hreader.h"
#include "hmetrics.h"

/**
    The processor makes a Hardt application tick, it reads from a given reader
    component and writes the data to a writer class.

    This allows you to split your DSP chain into one or more readers, and
    one or more writers. Readers sort of 'pull' data from reader endpoints and writers
    'push' data to writer endpoints (typical a HOutput type), but it can also be a
    HNullWriter if you dont need to have the output.

    When you have created your readers and writers, then create a processor with the
    last reader and first writer and then call Run() on the processor.

    A processor will stop when it receives a zero-length read or if you call the Halt() method,
    or the bool variable pointed to by the 'terminationToken' parameter of the constructor is
    set to true (especially usefull if you have multiple threads or does signal handling).
*/
template <class T>
class HProcessor : public HWriterConsumer<T>
{
    private:

        T* _buffer;
        int _blocksize;
        bool* _terminated;
        HMetrics _metrics;

    private:

        bool Start();

        bool Stop();

        void Init();

    protected:

        /**
         * The writer used by this processor
         */
        HWriter<T>* _writer;

        /**
         * The reader used by this processor
         */
        HReader<T>* _reader;

        /**
         * Construct a new HProcessor
         *
         * @param id Element identifier
         * @param writer The writer
         * @param reader The reader
         * @param blocksize Number of samples to read and write per cycle
         * @param terminationToken The termination token. Set to true to halt the processor
         */
        HProcessor(std::string id, HWriter<T>* writer, HReader<T>* reader, size_t blocksize, bool* terminationToken);

        /**
         * Construct a new HProcessor
         *
         * @param id Element identifier
         * @param reader The reader
         * @param blocksize Number of samples to read and write per cycle
         * @param terminationToken The termination token. Set to true to halt the processor
         */
        HProcessor(std::string id, HReader<T>* reader, size_t blocksize, bool* terminationToken);

        /**
         * Default destructor
         */
        ~HProcessor();

        /**
         * Set the writer
         *
         * @param writer The writer to use
         */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

    public:

        /**
         * Read directly from the associated reader
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         * @returns Number of blocks read
         */
        int ReadImpl(T* dest, int blocksize);

        /**
         * Write directly to the associated writer
         *
         * @param src Source buffer
         * @param blocksize Number of samples to write
         * @returns Number of blocks written
         */
        int WriteImpl(T* src, int blocksize);

        /**
         * Get the associated reader
         *
         * @returns A HReader*
         */
        HReader<T>* GetReader();

        /**
         * Get the associated writer
         *
         * @returns A HWriter*
         */
        HWriter<T>* GetWriter();

        /**
         * Read from the reader and write to the writer.
         *
         * Processing stops when a zero-length read is received,
         * halt() is called or the associated terminationToken is set to true.
         *
         * @param blocks Number of blocks to read or zero to read untill stopped (or null-read)
         */
        virtual void Run(long unsigned int blocks = 0);

        /**
         * Stop processing.
         *
         * If the reader and writer is not destroyed, processing can be resumed
         * by calling Run() once agan after setting the terminationToken to false
         */
        virtual void Halt();

        /**
         * Send a command to the reader and writer chain of this processor
         *
         * @param commandClass Class of command
         * @param commandOpcode Opcode of command
         * @param value integer value to send as the data
         * @returns True if the command was executed, false otherwise
         */
        bool Command(H_COMMAND_CLASS commandClass, H_COMMAND_OPCODE commandOpcode, int32_t value) {
            HCommandData data;
            data.Value = value;
            return Command(commandClass,commandOpcode, 0, data);
        }

        /**
         * Send a command to the reader and writer chain of this processor
         *
         * @param commandClass Class of command
         * @param commandOpcode Opcode of command
         * @param length Length of the data pointed to by 'content'
         * @param content void* to the data  to send as the data
         * @returns True if the command was executed, false otherwise
         */
        bool Command(H_COMMAND_CLASS commandClass, H_COMMAND_OPCODE commandOpcode, int16_t length, void* content) {
            HCommandData data;
            data.Content = content;
            return Command(commandClass,commandOpcode, length, data);
        }

        /**
         * Send a command to the reader and writer chain of this processor
         *
         * @param commandClass Class of command
         * @param commandOpcode Opcode of command
         * @param state Bool value to send as the data
         * @returns True if the command was executed, false otherwise
         */
        bool Command(H_COMMAND_CLASS commandClass, H_COMMAND_OPCODE commandOpcode, bool state) {
            HCommandData data;
            data.State = state;
            return Command(commandClass,commandOpcode, 0, data);
        }

        /**
         * Send a command to the reader and writer chain of this processor
         *
         * @param commandClass Class of command
         * @param commandOpcode Opcode of command
         * @param length The command data length (zero for int and bool values)
         * @param data The command data to send
         * @returns True if the command was executed, false otherwise
         */
        virtual bool Command(H_COMMAND_CLASS commandClass, H_COMMAND_OPCODE commandOpcode, int16_t length, HCommandData data) = 0;

        /**
         * Get the metrics object
         */
        HMetrics* GetMetrics() {
            return &_metrics;
        }
};

#endif