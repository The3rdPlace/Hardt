#ifndef __HSTREAMPROCESSOR_H
#define __HSTREAMPROCESSOR_H

#include "hreader.h"
#include "hwriter.h"

/**
    Read samples from a local reader and write them to a local writer
*/
template <class T>
class HStreamProcessor : public HProcessor<T>
{
    public:

        /** Construct a new HStreamProcessor */
        HStreamProcessor(HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken);

        /** Construct a new HStreamProcessor which will have the writer registered by a later
            writer, following the HWriterConsumer scheme */
        HStreamProcessor(HReader<T>* reader, int blocksize, bool* terminationToken);

        /** Run the processor */
        void Run(long unsigned int blocks = 0);

        /** Send a command */
        bool Command(HCOMMAND_CLASS commandClass, HCOMMAND_OPCODE commandOpcode, int16_t length, HCommandData data)
        {
            HCommand cmd = {static_cast<int16_t>(commandClass), static_cast<int16_t>(commandOpcode), length, data};

            if( HProcessor<T>::_reader != nullptr )
            {
                if( !HProcessor<T>::_reader->Command(&cmd) )
                {
                    HError("Error when sending command to reader chain");
                    return false;
                }
            }
            if( HProcessor<T>::_writer != nullptr )
            {
                if( !HProcessor<T>::_writer->Command(&cmd) )
                {
                    HError("Error when sending command to writer chain");
                    return false;
                }
            }
            return true;
        }
};

#endif