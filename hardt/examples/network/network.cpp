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

    // Todo: add more code
}

/**
    \example network.cpp
    How to link HReaders and HWriters over a network link
 */
