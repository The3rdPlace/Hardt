#ifndef __CONFIG_H
#define __CONFIG_H

#include <ctime>
#include <time.h>
#include "hsoundcard.h"
#include "htimer.h"
#include "hrtl2832reader.h"

// Settings
struct DspCmdConfig
{
    enum DeviceType { NONE = 0, AUDIO = 1, RTL = 2 };

    bool Verbose = false;
    bool ShowAudioDevices = false;
    bool ShowRtl2832Devices = false;

    bool IsNetworkReaderClient = false;
    bool IsNetworkWriterServer = false;
    bool IsSignalGenerator = false;
    bool IsFilePlayer = false;
    bool IsFileRecorder = false;
    bool IsFFTMagnitudeGnuPlot = false;
    bool IsFileConverter = false;
    bool IsClickRemoval = false;
    bool IsMixer = false;
    bool IsSubtracter = false;
    bool IsMultiplier = false;
    bool IsFilter = false;
    bool IsFilterSpectrumGnuPlot = false;
    bool IsBiQuadSpectrum = false;
    bool IsBiQuad = false;
    bool IsGain = false;
    bool IsComb = false;
    bool IsCombSpectrum = false;
    bool IsHum = false;
    bool IsHumSpectrum = false;
    bool IsGoertzl = false;
    bool IsBiQuadCascade = false;
    bool IsBiQuadCascadeSpectrum = false;
    bool IsMovingAverage = false;
    bool IsMovingAverageSpectrum = false;
    bool IsReal2Iq = false;
    bool IsFft = false;
    bool IsIfft = false;
    bool IsDemux = false;
    bool IsDecimator = false;
    bool IsUpsampler = false;
    bool IsInterpolator = false;
    bool IsKaiserBesselSpectrum = false;
    bool IsKaiserBessel = false;
    bool IsBaseband = false;
    bool IsTranslateByTwo = false;
    bool IsTranslateByFourI = false;
    bool IsTranslateByFourQ = false;
    bool IsFirDecimator = false;
    bool IsSampleTypeConverter = false;
    bool IsIq2Real = false;

    char* InFileFormat = NULL;
    char* OutFileFormat = NULL;

    H_SAMPLE_FORMAT SampleInType;
    H_SAMPLE_FORMAT SampleOutType;
    double TypeConverterScale = 1;

    int Port = 8080;
    char *Address = NULL;

    int FFTSize = 1024;
    int Average = 1;

    char *InputFile = NULL;
    char *OutputFile = NULL;
    char* FileFormat = NULL;
    bool ForceOverwrite = false;

    int InputDevice = -1;
    int InputDeviceType = DeviceType::AUDIO;
    int OutputDevice = -1;
    int Rate = H_SAMPLE_RATE_48K;
    int Format = H_SAMPLE_FORMAT_INT_16;
    int DirectSampling = false;
    HRtl2832::MODE Mode = HRtl2832::MODE::REAL;
    int Blocksize = 1024;
    int BlockCount = 0;

    int Frequency = 1000;
    float Phase = 0;
    float Alpha = 1;

    int Duration = 10;
    time_t Start = 0;
    HTimer Timer;

    char* InputFile1 = NULL;
    char* InputFile2 = NULL;

    char* FilterName = NULL;
    char* FilterCoeffs = NULL;

    float FCutOff;
    float Quality;
    float Gain;

    int M = 0;

    int Amplitude;

    int DecimateFactor;
    int UpsampleFactor;

    int FStart = 0;
    int FStop = 0;
    int Attenuation = 50;
    int Points = 257;

    int FCenter = 0;
    int ZoomFactor = 0;
    int Width = 0;
};

extern DspCmdConfig Config;

bool parseArguments(int argc, char** argv);

bool VerifyConfig();

#endif