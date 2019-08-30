/*
	Todo: add example
*/

#include <iostream>

#include "hardtapi.h"

int main(int argc, char** argv)
{
    // Initialize the Hardt toolkit.
    // Set the last argument to 'true' to enable verbose output instead of logging to a local file
    HInit(std::string("network"), true);

    // Show application name and and Hardt version.
    // This is not needed, just to have something on the screen
    std::cout << "network: using Hardt " + getversion() << std::endl;

    // Check that we got the required input parameters
    if( argc < 3 )
    {
        std::cout << "Usage: network 'output-sound-device-number'" << std::endl;
        std::cout << "Use 'dspcmd -a' to get a list of your sound device numbers" << std::endl;
    }

    // Decide on a block size to use - how many samples to pass into the chain on every
    // read or write. This mainly affects memory consumption but performance may be
    // reduced by large blocksizes since more data has to be moved between buffers
    const int BLOCKSIZE = 4096;

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for readers on the server
    //

    // Create a sinus generator running at 1KHz
    HSineGenerator<int16_t> generator(H_SAMPLE_RATE_48K, 1000, 50);

    // Increase the generator amplitude (just to add some readers)
    HGain<int16_t> gain(generator.Reader(), 2, BLOCKSIZE);

    // Setup a network processor that reads from the local readers and ships
    // the data to the remote end.
    //
    // A network processor can be configured to work in two directions:
    //   Server(read) ==> Client(write)
    //   Server(write) <== Client(read)
    //
    // This allows for shipping generated data out to a remote device, could be a soundcard or
    // a remote transmitter.
    bool serverTerminated = false;
    HNetworkProcessor<int16_t> serverProcessor(1928, gain.Reader(), BLOCKSIZE, &serverTerminated);

    // ------- NETWORK ---------

    // -------------------------------------------------------------------------------------
    // Setup dsp chain for writers
    //

    // Setup a network processor that reads from the remote server and writes to a local
    // writer. The downstream writers register using the HWriterConsumer interface.
    bool clientTerminated = false;
    HNetworkProcessor<int16_t> clientProcessor("127.0.0.1", 1928, BLOCKSIZE, &clientTerminated);

    // Create a fader that turns up the output volume when we begin to process samples.
    HFade<int16_t> fade(clientProcessor.Consumer(), 0, 500, true, BLOCKSIZE);

    // Create a soundcard writer, to output the received 1KHz signal
    HSoundcardWriter<int16_t> soundcard(atoi(argv[2]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, BLOCKSIZE, fade.Consumer());

    // -------------------------------------------------------------------------------------
    // Run

    // On the server, the processor is started as you would normally do. It will begin to read
    // data once you connect, and stop again when you disconnect.
    serverProcessor.Run();

    // On the client, Start the processor and receive 1000 blocks
    clientProcessor.Run(1000);

    // -------------------------------------------------------------------------------------
    // Cleanup - only on the server

    // Stop the processor
    serverProcessor.Halt();
}

/**
    \example network.cpp
    How to link HReaders and HWriters over a network link
 */
