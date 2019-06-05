#ifndef __CONFIG_H
#define __CONFIG_H

#include "hsoundcard.h"

// Settings
struct DspCmdConfig
{
    bool Verbose = false;
    bool ShowAudioDevices = false;

    bool IsNetworkReaderClient = false;
    bool IsNetworkWriterServer = false;
    bool IsSignalGenerator = false;
    bool IsFilePlayer = false;
    bool IsFileRecorder = false;
    bool IsFFTMagnitudeFlat = false;
    bool IsFFTMagnitudePlot = false;

    int Port = 8080;
    char *Address = NULL;

    int FFTSize = 1024;

    char *InputFile = NULL;
    char *OutputFile = NULL;
    char* FileFormat = NULL;

    int InputDevice = -1; // not implemented
    int OutputDevice = -1;
    int Rate = H_SAMPLE_RATE_48K;
    int Format = H_SAMPLE_FORMAT_INT_16;

    int Blocksize = 1024;

    int Frequency = 1000;
    float Phase = 0;
    int Duration = 10;
};

extern DspCmdConfig Config;

bool parseArguments(int argc, char** argv);

bool VerifyConfig();

#endif