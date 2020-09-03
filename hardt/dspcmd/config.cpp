#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <ctime>

#include "config.h"
#include "hardtapi.h"

DspCmdConfig Config;

static char PCM_FORMAT_STRING[] = "pcm";
static char  WAV_FORMAT_STRING[] = "wav";

bool argBoolCmp(const char* arg, const char* option, bool currentValue)
{
    return strcmp(arg, option) == 0 ? true : currentValue;
}

char* argCharCmp(const char* arg, const char* option, char* value, char* currentValue)
{
    return strcmp(arg, option) == 0 ? value : currentValue;
}

int argIntCmp(const char* arg, const char* option, char* value, int currentValue)
{
    return strcmp(arg, option) == 0 ? atoi(value) : currentValue;
}

float argFloatCmp(const char* arg, const char* option, char* value, float currentValue)
{
    return strcmp(arg, option) == 0 ? atof(value) : currentValue;
}

int argDeviceTypeCmp(const char* arg, const char* option, char* value, int currentValue)
{
    if( strcmp(arg, option) == 0 ) {
        if( strcmp(value, "AUDIO") == 0 ) return DspCmdConfig::DeviceType::AUDIO;
        if( strcmp(value, "RTL") == 0 ) return DspCmdConfig::DeviceType::RTL;
        return DspCmdConfig::DeviceType::NONE;
    }
    return currentValue;

}

bool parseArguments(int argc, char** argv)
{
    int PhaseIntValue = 0;

    for( int argNo = 1; argNo < argc; argNo++ )
    {
        Config.Verbose = argBoolCmp(argv[argNo], "-v", Config.Verbose);
        Config.ShowAudioDevices = argBoolCmp(argv[argNo], "-a", Config.ShowAudioDevices);
        Config.ShowRtl2832Devices = argBoolCmp(argv[argNo], "-t", Config.ShowRtl2832Devices);

        Config.IsFileRecorder = argBoolCmp(argv[argNo], "-rf", Config.IsFileRecorder);
        Config.IsFilePlayer = argBoolCmp(argv[argNo], "-pf", Config.IsFilePlayer);

        Config.ForceOverwrite = argBoolCmp(argv[argNo], "-force", Config.ForceOverwrite);

        Config.IsGoertzl = argBoolCmp(argv[argNo], "-gz", Config.IsGoertzl);

        Config.IsReal2Iq = argBoolCmp(argv[argNo], "-riq", Config.IsReal2Iq);

        Config.IsFft = argBoolCmp(argv[argNo], "-fft", Config.IsFft);
        Config.IsIfft = argBoolCmp(argv[argNo], "-ifft", Config.IsIfft);

        Config.IsDemux = argBoolCmp(argv[argNo], "-dmx", Config.IsDemux);

        Config.IsTranslateByTwo = argBoolCmp(argv[argNo], "-tr2", Config.IsTranslateByTwo);

        Config.IsTranslateByFourI = argBoolCmp(argv[argNo], "-tr4i", Config.IsTranslateByFourI);
        Config.IsTranslateByFourQ = argBoolCmp(argv[argNo], "-tr4q", Config.IsTranslateByFourQ);

        Config.IsFFTMagnitudeGnuPlot = argBoolCmp(argv[argNo], "-fmgp", Config.IsFFTMagnitudeGnuPlot);
        if( argNo < argc - 1 && argv[argNo + 1][0] != '-' ) {
            Config.FFTSize = argIntCmp(argv[argNo], "-fmgp", argv[argNo + 1], Config.FFTSize);
            if (argNo < argc - 3 && argv[argNo + 2][0] != '-') {
                Config.FCenter = argIntCmp(argv[argNo], "-fmgp", argv[argNo + 2], Config.FCenter);
                Config.ZoomFactor = argIntCmp(argv[argNo], "-fmgp", argv[argNo + 3], Config.ZoomFactor);
            }
        }

        if( argNo < argc - 1)
        {
            Config.InputFile = argCharCmp(argv[argNo], "-if", argv[argNo + 1], Config.InputFile);
            Config.OutputFile = argCharCmp(argv[argNo], "-of", argv[argNo + 1], Config.OutputFile);

            Config.Blocksize = argIntCmp(argv[argNo], "-bs", argv[argNo + 1], Config.Blocksize);
            Config.FFTSize = argIntCmp(argv[argNo], "-bs", argv[argNo + 1], Config.FFTSize); // also set default fft size
            Config.BlockCount = argIntCmp(argv[argNo], "-bc", argv[argNo + 1], Config.BlockCount);

            Config.InputDevice = argIntCmp(argv[argNo], "-id", argv[argNo + 1], Config.InputDevice);
            Config.InputDeviceType = argDeviceTypeCmp(argv[argNo], "-it", argv[argNo + 1], Config.InputDeviceType);
            Config.OutputDevice = argIntCmp(argv[argNo], "-od", argv[argNo + 1], Config.OutputDevice);
            Config.Rate = argIntCmp(argv[argNo], "-r", argv[argNo + 1], Config.Rate);
            Config.Format = argIntCmp(argv[argNo], "-f", argv[argNo + 1], Config.Format);

            Config.IsNetworkWriterServer = argBoolCmp(argv[argNo], "-ns", Config.IsNetworkWriterServer);
            Config.Port = argIntCmp(argv[argNo], "-ns", argv[argNo + 1], Config.Port);

            Config.FileFormat = argCharCmp(argv[argNo], "-ff", argv[argNo + 1], Config.FileFormat);

            if( argBoolCmp(argv[argNo], "-start", false) )
            {
                Config.Timer.SetStart(argv[argNo + 1]);
            }

            if( argBoolCmp(argv[argNo], "-stop", false) )
            {
                Config.Timer.SetStop(argv[argNo + 1]);
            }

            Config.Average = argIntCmp(argv[argNo], "-avg", argv[argNo + 1], Config.Average);

            Config.IsMultiplier = argBoolCmp(argv[argNo], "-mp", Config.IsMultiplier);
            Config.Frequency = argIntCmp(argv[argNo], "-mp", argv[argNo + 1], Config.Frequency);

            Config.IsGain = argBoolCmp(argv[argNo], "-g", Config.IsGain);
            Config.Gain = argFloatCmp(argv[argNo], "-g", argv[argNo + 1], Config.Gain);

            Config.IsBiQuadCascadeSpectrum = argBoolCmp(argv[argNo], "-bqc", Config.IsBiQuadCascadeSpectrum);
            Config.FilterCoeffs = argCharCmp(argv[argNo], "-bqc", argv[argNo + 1], Config.FilterCoeffs);

            Config.IsBiQuadCascade = argBoolCmp(argv[argNo], "-bqct", Config.IsBiQuadCascade);
            Config.FilterCoeffs = argCharCmp(argv[argNo], "-bqct", argv[argNo + 1], Config.FilterCoeffs);

            Config.IsMovingAverageSpectrum = argBoolCmp(argv[argNo], "-ma", Config.IsMovingAverageSpectrum);
            Config.M = argIntCmp(argv[argNo], "-ma", argv[argNo + 1], Config.M);

            Config.IsMovingAverage = argBoolCmp(argv[argNo], "-mat", Config.IsMovingAverage);
            Config.M = argIntCmp(argv[argNo], "-mat", argv[argNo + 1], Config.M);

            Config.IsDecimator = argBoolCmp(argv[argNo], "-dcm", Config.IsDecimator);
            Config.DecimateFactor = argIntCmp(argv[argNo], "-dcm", argv[argNo + 1], Config.DecimateFactor);

            Config.IsUpsampler = argBoolCmp(argv[argNo], "-up", Config.IsUpsampler);
            Config.UpsampleFactor = argIntCmp(argv[argNo], "-up", argv[argNo + 1], Config.UpsampleFactor);
        }

        if( argNo < argc - 2 )
        {
            Config.IsNetworkReaderClient = argBoolCmp(argv[argNo], "-nc", Config.IsNetworkReaderClient);
            Config.Address = argCharCmp(argv[argNo], "-nc", argv[argNo + 1], Config.Address);
            Config.Port = argIntCmp(argv[argNo], "-nc", argv[argNo + 2], Config.Port);

            Config.IsFileConverter = argBoolCmp(argv[argNo], "-fc", Config.IsFileConverter);
            Config.InFileFormat = argCharCmp(argv[argNo], "-fc", argv[argNo + 1], Config.InFileFormat);
            Config.OutFileFormat = argCharCmp(argv[argNo], "-fc", argv[argNo + 2], Config.OutFileFormat);

            Config.IsClickRemoval = argBoolCmp(argv[argNo], "-cr", Config.IsClickRemoval);
            Config.InFileFormat = argCharCmp(argv[argNo], "-cr", argv[argNo + 1], Config.InFileFormat);
            Config.OutFileFormat = argCharCmp(argv[argNo], "-cr", argv[argNo + 2], Config.OutFileFormat);

            Config.IsMixer = argBoolCmp(argv[argNo], "-mx", Config.IsMixer);
            Config.InputFile1 = argCharCmp(argv[argNo], "-mx", argv[argNo + 1], Config.InputFile1);
            Config.InputFile2 = argCharCmp(argv[argNo], "-mx", argv[argNo + 2], Config.InputFile2);

            Config.IsSubtracter = argBoolCmp(argv[argNo], "-sb", Config.IsSubtracter);
            Config.InputFile1 = argCharCmp(argv[argNo], "-sb", argv[argNo + 1], Config.InputFile1);
            Config.InputFile2 = argCharCmp(argv[argNo], "-sb", argv[argNo + 2], Config.InputFile2);

            Config.IsFilter = argBoolCmp(argv[argNo], "-flt", Config.IsFilter);
            Config.FilterName = argCharCmp(argv[argNo], "-flt", argv[argNo + 1], Config.FilterName);
            Config.FilterCoeffs = argCharCmp(argv[argNo], "-flt", argv[argNo + 2], Config.FilterCoeffs);

            Config.IsFilterSpectrumGnuPlot = argBoolCmp(argv[argNo], "-fl", Config.IsFilterSpectrumGnuPlot);
            Config.FilterName = argCharCmp(argv[argNo], "-fl", argv[argNo + 1], Config.FilterName);
            Config.FilterCoeffs = argCharCmp(argv[argNo], "-fl", argv[argNo + 2], Config.FilterCoeffs);

            Config.IsComb = argBoolCmp(argv[argNo], "-cbt", Config.IsComb);
            Config.Frequency = argIntCmp(argv[argNo], "-cbt", argv[argNo + 1], Config.Frequency);
            Config.Alpha = argFloatCmp(argv[argNo], "-cbt", argv[argNo + 2], Config.Alpha);

            Config.IsCombSpectrum = argBoolCmp(argv[argNo], "-cb", Config.IsCombSpectrum);
            Config.Frequency = argIntCmp(argv[argNo], "-cb", argv[argNo + 1], Config.Frequency);
            Config.Alpha = argFloatCmp(argv[argNo], "-cb", argv[argNo + 2], Config.Alpha);

            Config.IsHum = argBoolCmp(argv[argNo], "-hmt", Config.IsHum);
            Config.Frequency = argIntCmp(argv[argNo], "-hmt", argv[argNo + 1], Config.Frequency);
            Config.FCutOff = argIntCmp(argv[argNo], "-hmt", argv[argNo + 2], Config.FCutOff);

            Config.IsHumSpectrum = argBoolCmp(argv[argNo], "-hm", Config.IsHumSpectrum);
            Config.Frequency = argIntCmp(argv[argNo], "-hm", argv[argNo + 1], Config.Frequency);
            Config.FCutOff = argIntCmp(argv[argNo], "-hm", argv[argNo + 2], Config.FCutOff);

            Config.IsInterpolator = argBoolCmp(argv[argNo], "-ip", Config.IsInterpolator);
            Config.UpsampleFactor = argIntCmp(argv[argNo], "-ip", argv[argNo + 1], Config.UpsampleFactor);
            Config.FilterCoeffs = argCharCmp(argv[argNo], "-ip", argv[argNo + 2], Config.FilterCoeffs);

            Config.IsBaseband = argBoolCmp(argv[argNo], "-bb", Config.IsBaseband);
            Config.FCenter = argIntCmp(argv[argNo], "-bb", argv[argNo + 1], Config.FCenter);
            Config.Width = argIntCmp(argv[argNo], "-bb", argv[argNo + 2], Config.Width);

            Config.IsFirDecimator = argBoolCmp(argv[argNo], "-fdcm", Config.IsFirDecimator);
            Config.DecimateFactor = argIntCmp(argv[argNo], "-fdcm", argv[argNo + 1], Config.DecimateFactor);
            Config.FilterCoeffs = argCharCmp(argv[argNo], "-fdcm", argv[argNo + 2], Config.FilterCoeffs);

            Config.IsSampleTypeConverter = argBoolCmp(argv[argNo], "-stc", Config.IsSampleTypeConverter);
            Config.SampleInType = argIntCmp(argv[argNo], "-stc", argv[argNo + 1], Config.SampleInType);
            Config.SampleOutType = argIntCmp(argv[argNo], "-stc", argv[argNo + 2], Config.SampleOutType);
            if( argNo < argc - 3 && argv[argNo + 3][0] != '-' ) {
                Config.TypeConverterScale = argIntCmp(argv[argNo], "-stc", argv[argNo + 3], Config.TypeConverterScale);
            }
        }

        if( argNo < argc - 3 )
        {
        }

        if( argNo < argc - 4 )
        {
            Config.IsBiQuadSpectrum = argBoolCmp(argv[argNo], "-bq", Config.IsBiQuadSpectrum);
            Config.FilterName = argCharCmp(argv[argNo], "-bq", argv[argNo + 1], Config.FilterName);
            Config.FCutOff = argFloatCmp(argv[argNo], "-bq", argv[argNo + 2], Config.FCutOff);
            Config.Quality = argFloatCmp(argv[argNo], "-bq", argv[argNo + 3], Config.Quality);
            Config.Gain = argFloatCmp(argv[argNo], "-bq", argv[argNo + 4], Config.Gain);

            Config.IsBiQuad = argBoolCmp(argv[argNo], "-bqt", Config.IsBiQuad);
            Config.FilterName = argCharCmp(argv[argNo], "-bqt", argv[argNo + 1], Config.FilterName);
            Config.FCutOff = argFloatCmp(argv[argNo], "-bqt", argv[argNo + 2], Config.FCutOff);
            Config.Quality = argFloatCmp(argv[argNo], "-bqt", argv[argNo + 3], Config.Quality);
            Config.Gain = argFloatCmp(argv[argNo], "-bqt", argv[argNo + 4], Config.Gain);

            Config.IsSignalGenerator = argBoolCmp(argv[argNo], "-sg", Config.IsSignalGenerator);
            Config.Frequency = argIntCmp(argv[argNo], "-sg", argv[argNo + 1], Config.Frequency);
            PhaseIntValue = argIntCmp(argv[argNo], "-sg", argv[argNo + 2], PhaseIntValue);
            Config.Phase = ((float) PhaseIntValue / 360) * 2 * M_PI;
            Config.Amplitude = argIntCmp(argv[argNo], "-sg", argv[argNo + 3], Config.Amplitude);
            Config.Duration = argIntCmp(argv[argNo], "-sg", argv[argNo + 4], Config.Duration);
        }

        if( argNo < argc - 5 )
        {
            Config.IsKaiserBesselSpectrum = argBoolCmp(argv[argNo], "-kb", Config.IsKaiserBesselSpectrum);
            Config.FilterName = argCharCmp(argv[argNo], "-kb", argv[argNo + 1], Config.FilterName);
            Config.FStart = argIntCmp(argv[argNo], "-kb", argv[argNo + 2], Config.FStart);
            Config.FStop = argIntCmp(argv[argNo], "-kb", argv[argNo + 3], Config.FStop);
            Config.Attenuation = argIntCmp(argv[argNo], "-kb", argv[argNo + 4], Config.Attenuation);
            Config.Points = argIntCmp(argv[argNo], "-kb", argv[argNo + 5], Config.Points);

            Config.IsKaiserBessel = argBoolCmp(argv[argNo], "-kbt", Config.IsKaiserBessel);
            Config.FilterName = argCharCmp(argv[argNo], "-kbt", argv[argNo + 1], Config.FilterName);
            Config.FStart = argIntCmp(argv[argNo], "-kbt", argv[argNo + 2], Config.FStart);
            Config.FStop = argIntCmp(argv[argNo], "-kbt", argv[argNo + 3], Config.FStop);
            Config.Attenuation = argIntCmp(argv[argNo], "-kbt", argv[argNo + 4], Config.Attenuation);
            Config.Points = argIntCmp(argv[argNo], "-kbt", argv[argNo + 5], Config.Points);
        }

        if( argBoolCmp(argv[argNo], "-h", false) )
        {
            std::cout << "Usage: dspcmd [option [value]]" << std::endl << std::endl;

            std::cout << "$ dspcmd -a" << std::endl;
            std::cout << "Show a list of available audio devices" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dspcmd -t" << std::endl;
            std::cout << "Show a list of available RTL2832 devices" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dspcmd -h" << std::endl;
            std::cout << "Show this help" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dspcmd -v" << std::endl;
            std::cout << "Be verbose, dont write to logfiles but to stdout" << std::endl;
            std::cout << std::endl;

            std::cout << "Get further help for each option below in the documentation which is installed" << std::endl;
            std::cout << "locally here: /usr/local/share/doc/hardt/html/index.html, or on the Hardt website" << std::endl;
            std::cout << "found at http://hardttoolkit.org/dspcmd.html" << std::endl;
            std::cout << std::endl;

            std::cout << "Operations:" << std::endl;
            std::cout << "------------------------------------------------------------------------------------" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bs blocksize" << std::endl;
            std::cout << "Blocksize used by readers and writers (default = 1024)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bc count" << std::endl;
            std::cout << "Number of blocks read before stopping. Default 0 (unlimited)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -f format" << std::endl;
            std::cout << "Sample format (" << H_SAMPLE_FORMAT_INT_8 << "=Int8, " << H_SAMPLE_FORMAT_UINT_8 << "=UInt8, " << H_SAMPLE_FORMAT_INT_16 << "=Int16, " /*<< H_SAMPLE_FORMAT_INT_24 << "=Int24, "*/ << H_SAMPLE_FORMAT_INT_32 << "=Int32, " << H_SAMPLE_FORMAT_FLOAT << "=Float) (default " << H_SAMPLE_FORMAT_INT_16 << " = Int16)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -ff pcm|wav" << std::endl;
            std::cout << "Type of filereader/filewriterwriter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -id device" << std::endl;
            std::cout << "Input device id" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -it [AUDIO|RTL]" << std::endl;
            std::cout << "Input device type" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -if name" << std::endl;
            std::cout << "Name and path of input file" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -od device" << std::endl;
            std::cout << "Output device id" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -of name" << std::endl;
            std::cout << "Name and path of output file" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -force" << std::endl;
            std::cout << "Force overwrite of existing file" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -r rate" << std::endl;
            std::cout << "Sample rate (" << H_SAMPLE_RATE_8K << ", " << H_SAMPLE_RATE_11K << ", " << H_SAMPLE_RATE_22K << ", " << H_SAMPLE_RATE_32K << ", " << H_SAMPLE_RATE_44K1 << ", " << H_SAMPLE_RATE_48K << ", " << H_SAMPLE_RATE_96K << ", " << H_SAMPLE_RATE_192K << ") (default 48000)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -start [YYYY-MM-DD ]" << std::endl;
            std::cout << "hh:mm     Start time" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -stop [YYYY-MM-DD ]" << std::endl;
            std::cout << "hh:mm      Stop time" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -sg frequency phase amplitude duration" << std::endl;
            std::cout << "Generate a signal with the given frequency, phase and amplitude" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -nc server port" << std::endl;
            std::cout << "Run as network client, reading from the network and writing to a local writer" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -ns port" << std::endl;
            std::cout << "Run as network server, reading from a local reader and writing to the network" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -rf" << std::endl;
            std::cout << "Record file" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -pf" << std::endl;
            std::cout << "Play file" << std::endl;
            std::cout << std::endl;

            std::cout << "$ -fmgp [size [fcenter factor]]" << std::endl;
            std::cout << "Run FFT on a file and plot the magnitude spectrum on screen using GnuPlot" << std::endl;
            std::cout << "If a center frequency and a factor is given, zooming is applied." << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -fc in-format out-format" << std::endl;
            std::cout << "Convert input file format to output file format (same as in -ff)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -cr in-format out-format" << std::endl;
            std::cout << "Run the input file through a clickremoval filter." << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -mx file1 file2" << std::endl;
            std::cout << "Mix (linear) file1 and file2" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -sb file1 file2" << std::endl;
            std::cout << "Subtract file1 and file2" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -mp frequency" << std::endl;
            std::cout << "Multiply (mix nonlinear) with localoscilator signal running at frequency" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -flt name coeffs" << std::endl;
            std::cout << "Read coefficients from coeffs and run file through filter name (HIirFilter|HFirFilter)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -fl name coeffs" << std::endl;
            std::cout << "Read coefficients from coeffs and (Gnu)plot filter response for filter name  (HIirFilter|HFirFilter)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bq name Fcutoff Q G" << std::endl;
            std::cout << "Create biquad filter, dump coefficients and run sweep" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bqt name Fcutoff Q G" << std::endl;
            std::cout << "Create biquad filter and run file through the filter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bqc coeffs" << std::endl;
            std::cout << "Create cascaded biquad filter from the given coefficients in 'coeffs' and run sweep" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bgct coeffs" << std::endl;
            std::cout << "Create cascaded biquad filter from the given coefficients in 'coeffs' and run file through the filter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -kb name start stop att pts" << std::endl;
            std::cout << "Create Kaiser-Bessel filter, dump coefficients and run sweep" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -kbt name start stop att pts" << std::endl;
            std::cout << "Create Kaiser-Bessel filter and run file through the filter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -g gain" << std::endl;
            std::cout << "Run file through gain filter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -cb fBase alpha" << std::endl;
            std::cout << "Sweep a comb filter with base frequency fBase and alpha (gain)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -cbt fBase alpha" << std::endl;
            std::cout << "Run a file through a comb filter with base frequency fBase and alpha (gain)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -hm fBase fCutoff" << std::endl;
            std::cout << "Sweep a hum filter with base frequency fBase and cutoff frequency fCutoff" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -hmt fBase fCutoff" << std::endl;
            std::cout << "Run a file through a hum filter with base frequency fBase and cutoff frequency fCutoff" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -gz" << std::endl;
            std::cout << "Sweep a file by running the individual bins through a Goertzl filter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -ma num" << std::endl;
            std::cout << "Create moving average filter and run sweep" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -mat num" << std::endl;
            std::cout << "Create moving average filter and run file through the filter" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -riq" << std::endl;
            std::cout << "Convert realvalued samples to IQ samples" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -fft" << std::endl;
            std::cout << "Convert input signal to FFT" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -ifft" << std::endl;
            std::cout << "Convert input FFT to IFFT (signal)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -dmx" << std::endl;
            std::cout << "Demultiplex samples into two separate files" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -dcm factor" << std::endl;
            std::cout << "Decimate input samples by a factor" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -tr2" << std::endl;
            std::cout << "Translate input samples by two (invert spectrum)" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -tr4i" << std::endl;
            std::cout << "Translate input samples by four and output I branch" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -tr4q" << std::endl;
            std::cout << "Translate input samples by four and output Q branch" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -fdcm factor coeffs" << std::endl;
            std::cout << "Filter input file through a FIR decimator" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -up factor" << std::endl;
            std::cout << "Upsample input samples by a factor" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -ip factor coeffs" << std::endl;
            std::cout << "Interpolate input samples by a factor with FIR coefficients from 'coeffs'" << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -bb center width" << std::endl;
            std::cout << "Move segment around 'center' with 'width' to baseband." << std::endl;
            std::cout << std::endl;

            std::cout << "$ dpscmd -stc in out" << std::endl;
            std::cout << "Convert sample type. 'in' and 'out' must be one of the sample formats. See '-f'" << std::endl;
            std::cout << std::endl;

            std::cout << "When creating filters, 'name' is the corresponding API name for the filter or calculator.:" << std::endl;
            std::cout << "* Regular filters (-fl/-flt)......: HFirFilter, HIirFilter" << std::endl;
            std::cout << "* BiQuad filters (-bq/-bqt).......: HLowpassBiQuad, HhighpassBiQuad, HbandpassBiQuad, HNotchBiQuad, HAllpassBiQuad" << std::endl;
            std::cout << "                                    HPeakingEQBiQuad, HLowShelvingBiQuad, HHighShelvingBiQuad" << std::endl;
            std::cout << "* Kaiser-Bessel filter (-kb/-kbt).: HLowpassKaiserBessel, HHighpassKaiserBessel, HBandpassKaiserBessel, HBandstopKaiserBessel" << std::endl;
            std::cout << std::endl;

            // Force exit
            return true;
        }
    }

    // Arguments read, go ahead
    return false;
}

bool VerifyConfig()
{
    // We always needs a samplerate and a sample format
    if( Config.Rate == -1 )
    {
        std::cout << "No rate (-r)" << std::endl;
        return true;
    }
    if( Config.Format == -1 )
    {
        std::cout << "No format (-f)" << std::endl;
        return true;
    }

    // If we are writing a file, check if it exists
    if( Config.OutputFile != NULL && !Config.ForceOverwrite )
    {
        if( !!std::ifstream(Config.OutputFile) )
        {
            std::cout << "Output file exists!. use -force to overwrite" << std::endl;
            return true;
        }
    }

    // If we have input and/or output files but no file format, check if we can guess the format
    if( Config.InputFile != NULL && Config.FileFormat == NULL && strlen(Config.InputFile) > 4 )
    {
        if( strcmp( &Config.InputFile[strlen(Config.InputFile) - 4], ".pcm") == 0 )
        {
            Config.FileFormat = (char*) &PCM_FORMAT_STRING;
            Config.InFileFormat = (char*) &PCM_FORMAT_STRING;
        }
        else if( strcmp( &Config.InputFile[strlen(Config.InputFile) - 4], ".wav") == 0 )
        {
            Config.FileFormat = (char*) &WAV_FORMAT_STRING;
            Config.InFileFormat = (char*) &WAV_FORMAT_STRING;
        }
    }
    else if( Config.OutputFile != NULL && Config.FileFormat == NULL && strlen(Config.OutputFile) > 4 )
    {
        if( strcmp( &Config.OutputFile[strlen(Config.OutputFile) - 4], ".pcm") == 0 )
        {
            Config.FileFormat = (char*) &PCM_FORMAT_STRING;
            Config.OutFileFormat = (char*) &PCM_FORMAT_STRING;
        }
        else if( strcmp( &Config.OutputFile[strlen(Config.OutputFile) - 4], ".wav") == 0 )
        {
            Config.FileFormat = (char*) &WAV_FORMAT_STRING;
            Config.OutFileFormat = (char*) &WAV_FORMAT_STRING;
        }
    }
    if( (Config.InputFile != NULL || Config.OutputFile != NULL) && Config.FileFormat == NULL )
    {
        if( Config.IsFilePlayer || Config.IsFileRecorder || Config.IsClickRemoval || Config.IsFileConverter ) {
            std::cout << "No file format specified, and can not be guessed from the file extension" << std::endl;
            return true;
        }
    }

    // Also attempt to guess the InFileFormat and OutFileFormat specifiers for operations that takes input and output
    if( Config.InputFile != NULL && Config.OutputFile != NULL && strlen(Config.InputFile) > 4 && strlen(Config.OutputFile) > 4 )
    {
        if( strcmp( &Config.InputFile[strlen(Config.InputFile) - 4], ".pcm") == 0 )
        {
            Config.InFileFormat = (char*) &PCM_FORMAT_STRING;
        }
        else if( strcmp( &Config.InputFile[strlen(Config.InputFile) - 4], ".wav") == 0 )
        {
            Config.InFileFormat = (char*) &WAV_FORMAT_STRING;
        }
        if( strcmp( &Config.OutputFile[strlen(Config.OutputFile) - 4], ".pcm") == 0 )
        {
            Config.OutFileFormat = (char*) &PCM_FORMAT_STRING;
        }
        else if( strcmp( &Config.OutputFile[strlen(Config.OutputFile) - 4], ".wav") == 0 )
        {
            Config.OutFileFormat = (char*) &WAV_FORMAT_STRING;
        }
    }
    if( Config.InputFile != NULL && Config.InFileFormat == NULL )
    {
        if( Config.IsReal2Iq || Config.IsFft || Config.IsDemux || Config.IsDecimator || Config.IsUpsampler || Config.IsInterpolator ) {
            std::cout << "File formats for the in- and/or output file can not be guessed from the file extension" << std::endl;
            return true;
        }
    }
    if( Config.OutputFile != NULL && Config.OutFileFormat == NULL )
    {
        if( Config.IsIfft | Config.IsDecimator || Config.IsUpsampler || Config.IsInterpolator ) {
            std::cout << "File formats for the in- and/or output file can not be guessed from the file extension" << std::endl;
            return true;
        }
    }

    // Check that an input file exists
    if( Config.InputFile != NULL )
    {
        if( !( !!std::ifstream(Config.InputFile) ) )
        {
            std::cout << "Input file does not exist!." << std::endl;
            return true;
        }
    }

    return false;
}
