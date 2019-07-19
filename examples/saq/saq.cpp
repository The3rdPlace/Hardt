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
    if( argc < 4 )
    {
        std::cout << "Usage: saq 'filename' 'output-sound-device-number' 'lo-freq'" << std::endl;
        std::cout << "Use 'dspcmd -a' to get a list of your sound device numbers" << std::endl;
    }

    // Todo: Lock this down to the correct value when we are done fidling with the dsp chain
    int loFreq = atoi(argv[3]);

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for readers - first to last

    // Create a wave-file reader, reading a local file with a prerecorded saq transmission.
    // We know that the recording is 16 bit signed, 48KSps, mono
    HWavReader<int16_t> input(argv[1]);

    // Add hum filter to remove 50Hz harmonics and the very lowest part of the spectrum (incl. 50Hz)
    HHumFilter<int16_t> humFilter((HReader<int16_t>*) &input, H_SAMPLE_RATE_48K, 50, 500, 4096);

    // Mix down to IF = 1KHz
    HMultiplier<int16_t> multiplier((HReader<int16_t>*) &humFilter, H_SAMPLE_RATE_48K, loFreq, 4096);

    // General lowpass filtering, removes most signals above the IF
    HBiQuadFilter<int16_t>* lowpass = HBiQuadFactory< HLowpassBiQuad<int16_t>, int16_t >::Create((HReader<int16_t>*)  &multiplier, 2000, H_SAMPLE_RATE_48K, 0.7071f, 1, 4096);

    // Increase signal strength after filtering
    HGain<int16_t> amplifier((HReader<int16_t>*) lowpass, 1000, 4096);

    // Narrow butterworth bandpass filter, bandwidth 100Hz around 1000-1100. 4th. order, 4 biquads cascaded
    // Removes (almost) anything but the mixed down signal from SAQ (Grimeton)
    //
    // Designed using http://www.micromodeler.com/dsp/
    //
    // Do not forget that the a1 and a2 coefficients already has been multiplied by -1, this makes the IIR filter simpler
    float coeffs[20] =
    {
        0.06053979315740952, -0.12107958631481903, 0.06053979315740952, 1.9701579350811518, -0.9881958184253727,// b0, b1, b2, -a1, -a2
        0.125, -0.25, 0.125, 1.9780280925054692, -0.9952212910209018,// b0, b1, b2, -a1, -a2
        0.00048828125, 0.0009765625, 0.00048828125, 1.9683639531082289, -0.9877622267827567,// b0, b1, b2, -a1, -a2
        0.00048828125, 0.0009765625, 0.00048828125, 1.9742906058109615, -0.9947853486870636// b0, b1, b2, -a1, -a2
    };
    HCascadedBiQuadFilter<int16_t> bandpass((HReader<int16_t>*) &amplifier, coeffs, 20, 4096);

    // General lowpass filtering after mixing down to IF
    HBiQuadFilter<int16_t>* lowpass2 = HBiQuadFactory< HLowpassBiQuad<int16_t>, int16_t >::Create((HReader<int16_t>*)  &bandpass, 1000, H_SAMPLE_RATE_48K, 0.7071f, 1, 4096);

    // Final boost of signal
    HGain<int16_t> gain((HReader<int16_t>*) lowpass2, 10, 4096);

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for writers - last to first (there is only one in this example)

    // Create a soundcard writer, to output the final decoded transmission
    //HSoundcardWriter<int16_t> soundcard(atoi(argv[2]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, 4096);
    HWavWriter<int16_t> soundcard("out.wav", H_SAMPLE_FORMAT_INT_16, 1, H_SAMPLE_RATE_48K);

    // -------------------------------------------------------------------------------------
    // Create a processor that reads from the readers-chain and writes to the writers-chain

    // Processor: read from last gain boost and write to the soundcard writer. Run until EOF
    bool terminated = false;
    HStreamProcessor<int16_t> processor(&soundcard, (HReader<int16_t>*) &gain, 4096, &terminated);
    processor.Run();
}
