/*
	Example showing how to connect several readers and writers, by utilizing the
	HWriterConsumer interface.
*/

#include <iostream>

#include "hardtapi.h"

int main(int argc, char** argv)
{
    // Initialize the Hardt toolkit.
    // Set the last argument to 'true' to enable verbose output instead of logging to a local file
    HInit(std::string("readers_and_consumers"), true);

    // Show application name and and Hardt version.
    // This is not needed, just to have something on the screen
    std::cout << "readers_and_consumers: using Hardt " + getVersion() << std::endl;

    // Check that we got the required input parameters
    if( argc < 2 )
    {
        std::cout << "Usage: readers_and_consumers 'output-sound-device-number'" << std::endl;
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
    HBiQuadFilter<HHighpassBiQuad<int16_t>, int16_t> highpass("highpass biquad", multiplier.Reader(), 1000, H_SAMPLE_RATE_48K, 0.7071f, 1, BLOCKSIZE);    // -------------------------------------------------------------------------------------

    // Setup dsp chain for writers using the HWriterConsumer interface
    //
    // When you use the HWriterConsumer interface, you can create writers from first to last
    // since you pass a pointer to the previous HWriter consumer. The new writer then
    // registers with the previous writer (or processor).
    //
    // Since the compiler are not always able to correctly cast the given HWriter (or
    // processor) pointer to a HwriterConsumer*, you have two options:
    // - use a normal cast: splitter( (HWriterConsumer<int16_t>*) processor, ... );
    // - use a helper method: splitter( processor.Consumer(), ... );
    //
    // It is important to know the all writers (classes implementing the HWriter interface)
    // can also be used as a - or with a HWriterConsumer. And besides the writers, all 
    // processors also implement the HWriterConsumer interface.
    //
    // Allthough the HWriterConsumer scheme seems to be more complicated to understand, in daily use
    // it is actually a lot easier to read the final code, so give it a try!

    // Processor that reads from the last reader and writes to the first writer
    bool terminated = false;
    HStreamProcessor<int16_t> processor("processor", highpass.Reader(), BLOCKSIZE, &terminated);

    // Create a fader that turns up the output volume when we begin to process samples.
    HFade<int16_t> fade("fade", processor.Consumer(), 0, 3000, true, BLOCKSIZE);

    // Create a soundcard writer, to output the 500Hz + 1500Hz signal
    HSoundcardWriter<int16_t> soundcard("soundcard writer", atoi(argv[1]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, BLOCKSIZE, fade.Consumer());

    // -------------------------------------------------------------------------------------
    // Run

    // Start the processor and run 100 blocks
    processor.Run(100);
}

/**
    \example readers_and_consumers.cpp
    How to link multiple HReaders and HWriters in a chain using the HWriterConsumer interface
 */
