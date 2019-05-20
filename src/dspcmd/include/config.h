#ifndef __CONFIG_H
#define __CONFIG_H

#include "hsoundcard.h"

// Settings
struct DspCmdConfig
{
    bool Verbose = false;
    bool ShowAudioDevices = false;

    bool IsNetworkReaderClient = false;
    bool IsNetworkWriterClient = false; // not implemented
    bool IsNetworkReaderServer = false; // not implemented
    bool IsNetworkWriterServer = false;
    bool IsSignalGenerator = false;

    int Port = 8080;
    char *Address = NULL;

    char *InputFile = NULL;
    char *OutputFile = NULL;
    char* FileFormat = NULL;

    int InputDevice = -1; // not implemented
    int OutputDevice = -1;
    int Rate = H_SAMPLE_RATE_48K;
    int Format = H_SAMPLE_FORMAT_INT_16;

    int Blocksize = 1024;

    int Frequency;
    float Phase;
};

extern DspCmdConfig Config;

bool parseArguments(int argc, char** argv);

bool VerifyConfig();

#endif