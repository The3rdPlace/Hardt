/*
    This small application demonstrates how the signal from SAQ (Grimeton transmitter)
    can be received using a soundcard and a bit of dsp code, using the Hardt framework.

    This signal is part of the tuneup transmission, prior to sending the message, on
    june 30. 2019 at 10.30 (8.30 UTC)

    The provided input signal, 'saq_tuneup.wav' has been recorded using the dspcmd
    application that is part of the Hardt framework, received on a Raspberry zero with
    a 15 euro usb-coundcard and a miniwhip antenna (Google it). The file contains simply
    the raw 16 bit signed samples from the soundcard without any processing, not even
    hum filtering and amplification. So instead of using a wavfile reader, one could
    easily use a soundcard reader instead and the receiver would be 'Live'

    For fun, you could use the dspcmd application to inspect the spectrum if the
    input data "dspcmd -if saq_tuneup.wav -fmgp 1024 -pr 16000 19000", can you spot the
    signal? (hint: no, its not the big spike close to 17.2KHz)

    This demo application uses most common parts of the Hardt framework.
    Input and output, some dynamically calculated biquad filters, a precalculated
    cascade of biquads (using an online filter designer) and gain.
    The application demonstrates how to construct chains of readers and writers, and
    linking them together with a processor.
*/

#include <iostream>

#include "hardtapi.h"

int callback(HSignalLevelResult* result, size_t length)
{
    std::cout << "\b\b\bS " << result->S << std::endl;
    return length;
}

int main(int argc, char** argv)
{
    // Initialize the Hardt toolkit.
    // Set the last argument to 'true' to enable verbose output instead of logging to a local file
    HInit(std::string("saq"), false);

    // Show application name and and Hardt version.
    // This is not needed, just to have something on the screen
	std::cout << "saq: using Hardt " + getversion() << std::endl;
    std::cout << "S 0";

    // Check that we got the required input parameters
    if( argc < 3 )
    {
        std::cout << "Usage: saq 'filename' 'output-sound-device-number'" << std::endl;
        std::cout << "Use 'dspcmd -a' to get a list of your sound device numbers" << std::endl;
    }

    // Decide on a block size to use - how many samples to pass into the chain on every
    // read or write. This mainly affects memory consumption but performance may be
    // reduced by large blocksizes since more data has to be moved between buffers
    const int BLOCKSIZE = 4096;

    // Set mixer (multiplier local oscillator input frequency). This could also be 18.240 which
    // would give more or less the same result - allthough the highpass filter should be adjusted
    // then - otherwise the 17.200 signal we are looking for will be reduced.
    const int LOCAL_OSCILATOR = 16160;

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for readers - first to last

    // Create a wave-file reader, reading a local file with a prerecorded saq transmission.
    // We know that the recording is 16 bit signed, 48KSps, mono
    HWavReader<int16_t> input(argv[1]);

    // Add hum filter to remove 50Hz harmonics and the very lowest part of the spectrum (incl. 50Hz)
    // These components, which have very high levels, will completely botch the rest of the chain
    // if allowed through (50Hz input is here a.o., with an insanely high level)
    HHumFilter<int16_t> humFilter(&input, H_SAMPLE_RATE_48K, 50, 500, BLOCKSIZE);

    // Increase signal strength after mixing to avoid losses before filtering and mixing
    HGain<int16_t> gain(humFilter.Reader(), 100, BLOCKSIZE);

    // Highpass filter before mixing to remove some of the lowest frequencies that may
    // get mirrored back into the final frequency range and cause (more) distortion.
    // (In this receiver, the results are good when the cutoff frequency is located at the local oscillator frequency)
    HBiQuadFilter<HHighpassBiQuad<int16_t>, int16_t> highpass(gain.Reader(), LOCAL_OSCILATOR, H_SAMPLE_RATE_48K, 0.7071f, 1, BLOCKSIZE);

    // Mix down to the output frequency.
    // 17200Hz - 16160Hz = 1040Hz  (place it somewhere inside the bandpass filter pass region)
    HMultiplier<int16_t> multiplier(highpass.Reader(), H_SAMPLE_RATE_48K, LOCAL_OSCILATOR, BLOCKSIZE);

    // Narrow butterworth bandpass filter, bandwidth 100Hz around 1000-1100. 4th. order, 4 biquads cascaded
    // Removes (almost) anything but the mixed down signal from SAQ (Grimeton)
    //
    // Designed using http://www.micromodeler.com/dsp/
    //
    // Do not forget that the a1 and a2 coefficients should be multiplied by -1 in the input coefficients
    // (thus saving some cycles in the filter)
    float coeffs[] =
    {
        0.06053979315740952, -0.12107958631481903, 0.06053979315740952, 1.9701579350811518, -0.9881958184253727,// b0, b1, b2, -a1, -a2
        0.125, -0.25, 0.125, 1.9780280925054692, -0.9952212910209018,// b0, b1, b2, -a1, -a2
        0.00048828125, 0.0009765625, 0.00048828125, 1.9683639531082289, -0.9877622267827567,// b0, b1, b2, -a1, -a2
        0.00048828125, 0.0009765625, 0.00048828125, 1.9742906058109615, -0.9947853486870636// b0, b1, b2, -a1, -a2
    };
    HCascadedBiQuadFilter<int16_t> bandpass(multiplier.Reader(), coeffs, 20, BLOCKSIZE);

    // General lowpass filtering after mixing down to IF
    HBiQuadFilter<HLowpassBiQuad<int16_t>, int16_t> lowpass(bandpass.Reader(), 2000, H_SAMPLE_RATE_48K, 0.7071f, 1, BLOCKSIZE);

    // Final boost of signal (output volume)
    HGain<int16_t> volume(lowpass.Reader(), 200, BLOCKSIZE);

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for writers - last to first

    // Create a soundcard writer, to output the final decoded transmission
    HSoundcardWriter<int16_t> soundcard(atoi(argv[2]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, BLOCKSIZE);

    // Create a fader that turns up the output volume when we begin to process samples.
    // This hides a naste "Click" in the beginning of the file, and other spurious noise
    // coming from filters that needs to stabilize
    HFade<int16_t> fade(soundcard.Writer(), 0 , 1000, true, BLOCKSIZE);

    // Create a signal level writer so that we can show the current signal level
    auto wr = HCustomWriter<HSignalLevelResult>::Create(callback);
    HSignalLevel<int16_t> level(wr, 10);

    // Create a splitter that splits the samples into two streams,
    // one goes to the soundcard and the other to the signallevel indicator
    HSplitter<int16_t> splitter(&fade, &level);

    // -------------------------------------------------------------------------------------
    // Create a processor that reads from the readers-chain and writes to the writers-chain

    // Read from last gain boost and write to the soundcard. Runs until EOF
    bool terminated = false;
    HStreamProcessor<int16_t> processor(&splitter, volume.Reader(), BLOCKSIZE, &terminated);
    processor.Run();
}
