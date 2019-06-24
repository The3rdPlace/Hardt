#ifndef __CONFIG_H
#define __CONFIG_H

#include <ctime>
#include <time.h>
#include "hsoundcard.h"
#include "htimer.h"

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
    bool IsFFTMagnitudePlot = false;
    bool IsFFTMagnitudeGnuPlot = false;

    int Port = 8080;
    char *Address = NULL;

    int FFTSize = 1024;
    int Average = 1;
    int XMin = 0;
    int XMax = 0;
    int YMin = 0;
    int YMax = 0;

    char *InputFile = NULL;
    char *OutputFile = NULL;
    char* FileFormat = NULL;
    bool ForceOverwrite = false;

    int InputDevice = -1; // not implemented
    int OutputDevice = -1;
    int Rate = H_SAMPLE_RATE_48K;
    int Format = H_SAMPLE_FORMAT_INT_16;

    int Blocksize = 1024;

    int Frequency = 1000;
    float Phase = 0;

    int Duration = 10;
    time_t Start = 0;
    HTimer Timer;
};

extern DspCmdConfig Config;

bool parseArguments(int argc, char** argv);

bool VerifyConfig();

#endif