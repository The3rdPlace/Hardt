#include <iostream>

#include "hardtapi.h"

int main(int argc, char** argv)
{
    // Initialize the Hardt toolkit.
    // Set the last argument to 'true' to enable verbose output instead of logging to a local file
    HInit(std::string("saq"), false);

    // Show application name and and Hardt version.
    // This is not needed, just to have something on the screen
	std::cout << "saq: using Hardt " + getversion() << std::endl ;

    // Check that we got the required input parameters
    if( argc < 3 )
    {
        std::cout << "Usage: saq 'filename' 'output-sound-device-number'" << std::endl;
        std::cout << "Use 'dspcmd -a' to get a list of your sound device numbers" << std::endl;
    }

    // ------------------------------------------------------------------------
    // Setup dsp chain

    // Create a wave-file reader, reading a local file with a prerecorded saq transmission.
    // We know that the recording is 16 bit signed, 48KSps, mono
    HWavReader<int16_t> input(argv[1]);

    // Todo: Add filters, mixers etc. here
    
    // Create a soundcard writer, to output the final decoded transmission
    // We would like to pass 4096 samples per write, to avoid too much overhead when moving data to the card
    HSoundcardWriter<int16_t> soundcard(atoi(argv[2]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, 4096);

    // Create a processor that reads from the reader-chain and writes to the writer-chain
    // Set the bool variable 'terminated' to stop the processor thread immediately
    bool terminated = false;
    HStreamProcessor<int16_t> processor(&soundcard, &input, 4096, &terminated);

    // ------------------------------------------------------------------------
    // Run: Read the entire file and output the decoded signal
    processor.Run();
}
