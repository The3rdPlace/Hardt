/*
	(very) simple example that shows how to create 1 generator, a reader, a processor and a writer.
	This is as simple as it gets.
*/

#include <iostream>

#include "hardtapi.h"

int main(int argc, char** argv)
{
    // Initialize the Hardt toolkit.
    // Set the last argument to 'true' to enable verbose output instead of logging to a local file
    HInit(std::string("readers_and_writers"), true);

    // Show application name and and Hardt version.
    // This is not needed, just to have something on the screen
    std::cout << "readers_and_writers: using Hardt " + getversion() << std::endl;

    // Check that we got the required input parameters
    if( argc < 2 )
    {
        std::cout << "Usage: simple 'output-sound-device-number'" << std::endl;
        std::cout << "Use 'dspcmd -a' to get a list of your sound device numbers" << std::endl;
        return 1;
    }

    // Decide on a block size to use - how many samples to pass into the chain on every
    // read or write. This mainly affects memory consumption but performance may be
    // reduced by large blocksizes since more data has to be moved between buffers
    const int BLOCKSIZE = 4096;

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for readers
    //
    // Readers are generators, soundcard input and file-readers. Filters a.o. can be
    // used as either readers or writers depending on how they fit best into the
    // chain being constructed - but we use no filters here

    // Create a sinus generator running at 1KHz
    HSineGenerator<int16_t> generator(H_SAMPLE_RATE_48K, 1000, 20000);

    // -------------------------------------------------------------------------------------
    // Setup a processor
    //
    // Each dsp chain needs 1 processor to get the samples to pull (read) samples from
    // the readers part of the chain and push (write) them through the writers part of the chain

    // Create a processor that reads from the last reader and writes to the first writer
    bool terminated = false;
    HStreamProcessor<int16_t> processor(generator.Reader(), BLOCKSIZE, &terminated);

    // Setup dsp chain for writers
    //
    // Writers are file-writers, soundcard output or one of the HOutput classes which
    // produces a non-sampled output upon the base of a stream of samples (fft spectrum,
    // signal level monitors etc.)
    //
    // We utilize the HWriterConsumer interface so that we can create the processor and
    // the writer in the direction of the dataflow, which makes it more easy to understand
    // the chain once created.

    // Create a soundcard writer, to output the 1KHz signal
    HSoundcardWriter<int16_t> soundcard(atoi(argv[1]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, BLOCKSIZE, processor.Consumer());


    // -------------------------------------------------------------------------------------
    // Run

    // Start the processor and run 100 blocks
    processor.Run(100);
}

/**
    \example simple.cpp
    How to link HReaders and HWriters in a chain
 */
