/*
	Example showing how to connect readers and writers over the network
	using a HNetworkProcessor
*/

#include <iostream>
#include <thread>

#include "hardtapi.h"

int main(int argc, char** argv)
{
    // Initialize the Hardt toolkit.
    // Set the last argument to 'true' to enable verbose output instead of logging to a local file
    HInit(std::string("network"), true);

    // Show application name and and Hardt version.
    // This is not needed, just to have something on the screen
    std::cout << "network: using Hardt " + getVersion() << std::endl;

    // Check that we got the required input parameters
    if( argc < 2 )
    {
        std::cout << "Usage: network 'output-sound-device-number'" << std::endl;
        std::cout << "Use 'dspcmd -a' to get a list of your sound device numbers" << std::endl;
        return 1;
    }

    // Decide on a block size to use - how many samples to pass into the chain on every
    // read or write. This mainly affects memory consumption but performance may be
    // reduced by large blocksizes since more data has to be moved between buffers
    const int BLOCKSIZE = 4096;

    // -------------------------------------------------------------------------------------
    // ONLY ON THE SERVER
    //
    // Setup dsp chain for readers on the server
    //

    // Create a sinus generator running at 1KHz
    HSineGenerator<int16_t> generator("server sine generator", H_SAMPLE_RATE_48K, 1000, 10000);

    // Increase the generator amplitude (just to add some readers)
    HGain<int16_t> gain("server gain", generator.Reader(), 2, BLOCKSIZE);

    // Setup a network processor that reads from the local readers and ships
    // the data to the remote end.
    //
    // A network processor can be configured to work in two directions:
    //   Server(read) ==> Client(write)
    //   Server(write) <== Client(read)
    // This processor will wait for a connection, then begin to read from the local
    // reader chain and put the data onto the network.

    // Create the servers processor
    bool serverTerminated = false;
    HNetworkProcessor<int16_t> serverProcessor("server processor", 1928, 1929, gain.Reader(), BLOCKSIZE, &serverTerminated);

    // On the server, the processor is started as you would normally do. It will begin to read
    // data once you connect, and stop again when you disconnect.
    //
    // For us to run this example from a single app, we need to run the server in a thread.
    // Normally, you would have the server and the client split into two applications, and
    // then all you have to do is to call: serverProcessor.Run();
    std::thread server([&serverProcessor]() {serverProcessor.Run(); });

    // ------- NETWORK ---------

    // -------------------------------------------------------------------------------------
    // ONLY ON THE CLIENT
    //
    // Setup dsp chain for writers
    //

    // Setup a network processor that reads from the remote server and writes to a local
    // writer. The downstream writers register using the HWriterConsumer interface.
    //
    // WARNING: Do not forget to add either the id og the host address, otherwise you'll
    //          create a server on http://localhost with the id <expected server hostname>
    bool clientTerminated = false;
    HNetworkProcessor<int16_t> clientProcessor("client processor", "127.0.0.1", 1928, 1929, BLOCKSIZE, &clientTerminated);

    // Create a fader that turns up the output volume when we begin to process samples.
    HFade<int16_t> fade("client fade", clientProcessor.Consumer(), 0, 3000, true, BLOCKSIZE);

    // Create a soundcard writer, to output the received 1KHz signal
    HSoundcardWriter<int16_t> soundcard("client soundcard writer", atoi(argv[1]), H_SAMPLE_RATE_48K, 1, H_SAMPLE_FORMAT_INT_16, BLOCKSIZE, fade.Consumer());

    // -------------------------------------------------------------------------------------
    // Run

    // On the client, Start the processor and receive 100 blocks
    clientProcessor.Run(100);

    // -------------------------------------------------------------------------------------
    // ONLY ON THE SERVER
    //
    // Cleanup: Stop the server listening for connections

    // Stop the processor
    serverProcessor.Halt();

    // Just in this example, wait for the server thread to finish
    server.join();
}

/**
    \example network.cpp
    How to link HReaders and HWriters over a network link
 */
