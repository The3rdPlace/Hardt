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

    // Add hum filter to remove 50Hz harmonics and the very lowest part of the spectrum (incl. 50Hz)
    HHumFilter<int16_t> humFilter((HReader<int16_t>*) &input, H_SAMPLE_RATE_48K, 50, 1000, 4096);

    // Add a bandpass filter around 17.2 KHZ as a preselect
    HBiQuadFilter<int16_t>* bandpass = HBiQuadFactory< HBandpassBiQuad<int16_t>, int16_t >::Create((HReader<int16_t>*)  &humFilter, 17200, H_SAMPLE_RATE_48K, 0.9f, 1, 4096);

    // Increase signal strength after filtering
    HGain<int16_t> amplifier((HReader<int16_t>*) bandpass, 100, 4096);

    // Todo: Add filters, mixers etc. here
    HMultiplier<int16_t> multiplier(&amplifier, H_SAMPLE_RATE_48K, 16200, 4096);

    // General lowpass filtering
    HBiQuadFilter<int16_t>* lowpass = HBiQuadFactory< HLowpassBiQuad<int16_t>, int16_t >::Create((HReader<int16_t>*)  &multiplier, 1000, H_SAMPLE_RATE_48K, 0.7071f, 1, 4096);

    // Create a very narrow bandpass filter around 1khz - a peak biquad
    float coeffs[] = { -1.848823142275648,   // a1
                        0.8647765642900187,  // a2
                        1.0672913948313805,  // b0
                       -1.848823142275648,   // b1
                        0.797485169458638 }; // b2
    HIirFilter<int16_t> peak((HReader<int16_t>*) lowpass, coeffs, 5, 4096);

    // Increase signal strength after peak filtering
    HGain<int16_t> gain((HReader<int16_t>*) &peak, 3, 4096);

    // Create a soundcard writer, to output the final decoded transmission
    // We would like to pass 4096 samples per write, to avoid too much overhead when moving data to the card
    HSoundcardWriter<int16_t> soundcard(atoi(argv[2]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, 4096);

    // Create a processor that reads from the reader-chain and writes to the writer-chain
    // Set the bool variable 'terminated' to stop the processor thread immediately
    bool terminated = false;
    HStreamProcessor<int16_t> processor(&soundcard, &gain, 4096, &terminated);

    // ------------------------------------------------------------------------
    // Run: Read the entire file and output the decoded signal
    processor.Run();
}
