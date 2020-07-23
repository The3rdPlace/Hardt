/*
	A very basic template for starting a new Hardt-based project

    Dont forget to install PortAudio 'sudo apt install libportaudio2'

    Run these commands to build and run:
    '''
        cmake .
        make
        ./MyHardtApp
    '''
*/

#include <iostream>
#include <signal.h>

#include "hardtapi.h"

#define FREQUENCY    1000
#define AMPLITUDE    20000

#define BLOCKSIZE    1024
#define RATE         H_SAMPLE_RATE_44K1
#define FORMAT       H_SAMPLE_FORMAT_INT_16

bool terminated = false;

static void signalIntTermHandler (int signal_value)
{
    terminated = true;
}

void InstallSignalHandler() {
    struct sigaction action;
    action.sa_handler = [](int) { terminated = true; };
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}

int main(int argc, char** argv) {

    // Install signalhandler to catch ctrl+c to quit
    InstallSignalHandler();

    // Initialize the Hardt toolkit with debugging disabled
    HInit(std::string("readers_and_writers"), false);

    // Show application name and and Hardt version.
    std::cout << APPNAME  << ": using Hardt " + getversion() << std::endl;

    // Create a sinus generator running at 1KHz and peak amplitude 20.000
    HSineGenerator<int16_t> generator(RATE, FREQUENCY, AMPLITUDE);

    // Create a processor
    HStreamProcessor<int16_t> processor(generator.Reader(), BLOCKSIZE, &terminated);

    // Create a soundcard writer, to output the 1KHz signal to the default sound device
    try {
        HSoundcardWriter<int16_t> soundcard(HSoundcard::GetDefaultDevice(), RATE, 1, FORMAT, BLOCKSIZE, processor.Consumer());
    }
    catch( HAudioIOException *ex ) {
        std::cout << "HAudioIOException: " << ex->what() << "\n";
    }
    catch( HException *hex ) {
        std::cout << "HException: " << hex->what() << "\n";
    }
    catch( ... ) {
        std::cout << "Unknown exception type caught\n";
    }

    // Start the processor and run untill terminated
    processor.Run();

    // Finished
    return 0;
}

/**
    \example main.cpp
    Example code for starting a Hardt-based project
 */
