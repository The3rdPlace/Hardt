/*
    Example showing how to connect several readers and writers
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
    std::cout << "readers_and_writers: using Hardt " + getVersion() << std::endl;

    // Check that we got the required input parameters
    if( argc < 2 )
    {
        std::cout << "Usage: readers_and_writers 'output-sound-device-number'" << std::endl;
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
    // Readers are created the expected way, first to last.
    // Each reader requires a pointer to the previous reader to be passed as one of
    // the arguments.. In many cases you can just pass in a pointer
    // to the previous reader object and let the compiler resolve the object type.
    // However, this does not always work, so in that case you have two options:
    // - use a normal cast: gain( (HReader<int16_t>) generator, ... );
    // - use a helper method: gain( generator.Reader(), ... );

    // Create a sinus generator running at 1KHz
    HSineGenerator<int16_t> generator("sine generator", H_SAMPLE_RATE_48K, 1000, 10000);

    // Increase the generator amplitude (just to add some readers)
    HGain<int16_t> gain("gain", generator.Reader(), 2, BLOCKSIZE);

    // Mix with a 500Hz tone
    // ==> 1000Hz - 500Hz = 500Hz
    // ==> 1000Hz + 500Hz = 1500Hz
    HMultiplier<int16_t> multiplier("multiplier", gain.Reader(), H_SAMPLE_RATE_48K, 500, 10, BLOCKSIZE);

    // General highpass filtering after mixing to make the 1500Hz tone the dominant
    HBiQuadFilter<HHighpassBiQuad<int16_t>, int16_t> highpass("highpass filter", multiplier.Reader(), 1000, H_SAMPLE_RATE_48K, 0.7071f, 1, BLOCKSIZE);

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for writers
    //
    // When you create writers without using the HWriterConsumer interface, you can must create the writers
    // from last to first since each writer must know where to write.
    //
    // Since the compiler are not always able to correctly cast the given HWriter (or
    // processor) pointer to a Hwriter*, you have two options:
    // - use a normal cast: splitter( (HWriter<int16_t>*) processor, ... );
    // - use a helper method: splitter( processor.Writer(), ... );
    //
    // Allthough this is the simplest scheme to get the writers connected, I find it very confusing
    // having to read 'from the bottom up'. Have a look on the 'readers_and_consumers' example to see
    // how it can be done in reverse.

    // Create a soundcard writer, to output the 500Hz + 1500Hz signal
    HSoundcardWriter<int16_t> soundcard("soundcard output", atoi(argv[1]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, BLOCKSIZE);

    // Create a fader that turns up the output volume when we begin to process samples.
    HFade<int16_t> fade("fader", soundcard.Writer(), 0, 3000, true, BLOCKSIZE);

    // Processor that reads from the last reader and writes to the first writer
    bool terminated = false;
    HStreamProcessor<int16_t> processor("processor", fade.Writer(), highpass.Reader(), BLOCKSIZE, &terminated);

    // -------------------------------------------------------------------------------------
    // Run

    // Start the processor and run 100 blocks
    processor.Run(100);
}

/**
    \example readers_and_writers.cpp
    How to link multiple HReaders and HWriters in a chain
 */
