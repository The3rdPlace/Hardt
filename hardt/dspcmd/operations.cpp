#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <limits>

#include "operations.h"
#include "signalhandling.h"
#include "config.h"
#include "hardtapi.h"

template <class T>
double GetCalibratedReference(double fdelta, int start, int stop, int amplitude, bool shrink = true) {
    int freq = start;
    HVfo<T> vfo("vfo", Config.Rate, freq, amplitude, 0);

    // Calibration - measure input spectrum magnitude when passed directly to the FFT
    const int Blocks = 128;
    HHahnWindow<T> window;
    HFft<T> fft(Blocks, &window);
    double refSpectrum[Blocks / 2];
    T refBlock[Blocks];
    double refSum = 0;
    int refNumFfts = 0;
    double refSummer[Blocks / 2];
    memset((void *) refSummer, 0, sizeof(double) * (Blocks / 2));
    for (; freq < stop; freq += fdelta) {

        vfo.SetFrequency(freq);

        vfo.Read(refBlock, Blocks);
        fft.FFT(refBlock, refSpectrum);

        for (int i = 0; i < Blocks / 2; i++) {
            refSummer[i] += refSpectrum[i];
        }
        refNumFfts++;
    }
    for (int i = 0; i < Blocks / 2; i++) {
        refSum += refSummer[i] / refNumFfts;
    }

    return (refSum / (Blocks / 2)) * (shrink ? ((Config.Rate / 2) / (stop - start)) : 1);
}

template <typename T>
int RunNetworkWriterServer()
{
    HSoundcardReader<T> rdr("soundcard reader", Config.InputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    HNetworkProcessor<T> srv("network processor", Config.Port, Config.Port + 1, &rdr, Config.Blocksize, &terminated);
    try
    {
        srv.Run();
        std::cout << srv.GetMetrics()->ToString();
    }
    catch( std::exception ex )
    {
        std::cout << "Caught exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

template <typename T>
int RunNetworkReaderClient()
{
    // Create requested writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", "filewriter", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create a network processor and run the client
    HNetworkProcessor<T> client("client processor", Config.Address, Config.Port, Config.Port + 1, wr, Config.Blocksize, &terminated);
    try
    {
        if( Config.Timer.Duration() > 0 )
        {
            unsigned long int blocks = (Config.Timer.Duration() * Config.Rate) / Config.Blocksize;
            client.Run(blocks);
        }
        else
        {
            client.Run();
        }
    }
    catch( std::exception ex )
    {
        std::cout << "Caught exception: " << ex.what() << std::endl;
        return 1;
    }

    // Delete writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }
    return 0;
}

template <typename T>
int RunSignalGenerator()
{
    // Create requested writer
    HWriter<T>* wr;
    if( Config.OutputDevice != -1 )
    {
        wr = new HSoundcardWriter<T>("soundcard writer", Config.OutputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    }
    else if ( Config.OutputFile != NULL )
    {
        if( strcmp(Config.FileFormat, "wav") == 0 )
        {
            wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
        }
        else if( strcmp(Config.FileFormat, "pcm") == 0 )
        {
            wr = new HFileWriter<T>("file writer", Config.OutputFile);
        }
        else
        {
            std::cout << "Unknown file format " << Config.FileFormat << std::endl;
            return -1;
        }
    }
    else
    {
        std::cout << "No output file or device" << std::endl;
        return -1;
    }

    // Calculate how many blocks fit into the requested duration
    unsigned long int blocks = (Config.Duration * Config.Rate) / Config.Blocksize;

    // Generate a signal with the requested amplitude
    HLog("Using amplitude %d", Config.Amplitude);

    // Create and run the signalgenerator
    HVfo<T> sg("vfo", Config.Rate, Config.Frequency, Config.Amplitude, Config.Phase);
    HStreamProcessor<T> proc("stream processor", wr, &sg, Config.Blocksize, &terminated);
    proc.Run(blocks);

    // Delete writer
    if( Config.OutputDevice != -1 )
    {
        delete (HSoundcardWriter<T>*) wr;
    }
    else
    {
        if( strcmp(Config.FileFormat, "wav") == 0 )
        {
            delete (HWavWriter<T>*) wr;
        }
        else if( strcmp(Config.FileFormat, "pcm") == 0 )
        {
            delete (HFileWriter<T>*) wr;
        }
    }

    return 0;
}

template <typename T>
int RunFileRecorder()
{
    // Create reader
    HReader<T>* rd;
    if( Config.InputDeviceType == DspCmdConfig::DeviceType::AUDIO )
    {
        rd = new HSoundcardReader<T>("sound card reader", Config.InputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    }
    else if ( Config.InputDeviceType == DspCmdConfig::DeviceType::RTL )
    {
        rd = new HRtl2832Reader<T>("rtl 2832 reader", Config.InputDevice, Config.Rate, Config.Mode, Config.Gain, Config.Frequency, Config.Blocksize);
    }
    else
    {
        std::cout << "Unknown input device type " << Config.InputDeviceType << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, rd, Config.Blocksize, &terminated);
    if( Config.Timer.Duration() > 0 )
    {
        unsigned long int blocks = (Config.Timer.Duration() * Config.Rate) / Config.Blocksize;
        proc.Run(blocks);
    }
    else
    {
        proc.Run(Config.BlockCount);
    }

    // Delete writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }

    // Delete input device
    if( Config.InputDeviceType == DspCmdConfig::DeviceType::AUDIO )
    {
        delete (HSoundcardReader<T>*) rd;
    }
    else if ( Config.InputDeviceType == DspCmdConfig::DeviceType::RTL )
    {
        delete (HRtl2832Reader<T>*) rd;
    }

    return 0;
}

template <typename T>
int RunFilePlayer()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HSoundcardWriter<T> wr("soundcard writer", Config.OutputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", &wr, rd, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete reader
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }

    return 0;
}

double *aggregatedMagnitudeSpectrum;
int numFfts = 0;

int FFTMagnitudePlotWriter(HFftResults* data, size_t size)
{
    for( int i = 0; i < data[0].Size; i++ )
    {
        aggregatedMagnitudeSpectrum[i] += data[0].Spectrum[i];
    }
    numFfts++;
    return size;
}

template <class T>
double NormalizeFFTPlot(double reference, double ignore, double* values, double* normalizedValues) {

    // Calculate scaling factors so that values are more or less the same
    // for any blocksize. 1024 is choosen as the default blocksize thus
    // setting the scaling level
    double scale = ((double) 1024 / (double) Config.Blocksize) * (double) numFfts;
    double scaledIgnore = ignore;

    // Scale fft magnitude values
    for( int i = 0; i < Config.FFTSize / 2; i++ )
    {
        normalizedValues[i] = ((double) values[i]) / scale;

        // Remove the lowest values since they are the result of quantization errors
        if( abs(normalizedValues[i]) < scaledIgnore) {
            normalizedValues[i] = 0;
        }

        // values below 0 is noise and must be removed
        if( normalizedValues[i] < 0 ) {
            normalizedValues[i] = 0;
        }
    }

    // Return reference value
    return reference;
}

template <class T>
void FFTMagnitudeShowGnuDBPlot(double reference, double ignore, bool skipInfinite = true)
{
    // Calculate the output spectrum
    double displayedSpectrum[Config.FFTSize / 2];
    double ref = NormalizeFFTPlot<T>(reference, ignore, aggregatedMagnitudeSpectrum, displayedSpectrum);

    // Use zooming ?
    int rate = Config.Rate;
    if( Config.ZoomFactor > 0 && Config.FCenter > 0 ) {
        rate /= Config.ZoomFactor;
        Config.FStart = Config.FCenter - ((rate / 2) / 2);
    }

    // Calculate frequency span per bin
    double fdelta = ((double) rate / 2) / ((double)Config.FFTSize / (Config.IsIq ? 4 : 2));
    if( fdelta < 1 ) {
        std::cout << "Too low fft size. Unable to display plot! (reduce blocksize)" << std::endl;
        return;
    }

    // Plot lines
    FILE* gnuplotPipe = popen ("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2 pointtype -1\n");
    fprintf(gnuplotPipe, "set xlabel \"Hz\" offset 0,0\n");
    fprintf(gnuplotPipe, "set ylabel \"DB\" offset 0,0\n");
    fprintf(gnuplotPipe, "plot '-' with linespoints linestyle 1 title \"%d point fft\"\n", Config.Blocksize);
    if( Config.IsIq ) {
        for (int bin = (Config.FFTSize / 4); bin < (Config.FFTSize / 2) - 1; bin++)
        {
            double binFrequency = Config.FStart - ((Config.FFTSize / 2) - (double) bin) * fdelta;

            // Plot DB value
            double ratio = displayedSpectrum[bin] / ref;
            double db = (double) 20 * log10(ratio);
            if (isinff(db)) {
                // skip infinitely small values ?(with respect to the discrete sample)
                if (skipInfinite) {
                    continue;
                } else {
                    db = 0;
                }
            }
            if (!isnan(db)) {
                fprintf(gnuplotPipe, "%lf %lf\n", binFrequency, db);
            }
        }
    }
    for( int bin = 0; bin < (Config.FFTSize / (Config.IsIq ? 4 : 2)); bin++ )
    {
        double binFrequency = ((double) bin * fdelta) + Config.FStart;

        // Plot DB value
        double ratio = displayedSpectrum[bin] / ref;
        double db = (double) 20 * log10(ratio);

        if( isinff(db) ) {
            // skip infinitely small values ?(with respect to the discrete sample)
            if( skipInfinite ) {
                continue;
            } else {
                db = 0;
            }
        }
        if( !isnan(db) ) {
            fprintf(gnuplotPipe, "%lf %lf\n", binFrequency, db);
        }
    }
    fprintf(gnuplotPipe, "e");
    fclose(gnuplotPipe);
}

template <class T>
void FFTMagnitudeShowGnuDBPlot(double reference)
{
    // Scale when using other blocksizes than 1024
    double ref = reference * ((double) Config.Blocksize / (double) 1024);

    // Calculate minimum acceptable value (remove noise near zero)
    double ignore = (std::numeric_limits<T>::max() / 30);

    // Plot the spectrum
    FFTMagnitudeShowGnuDBPlot<T>(ref, ignore);
}

template <class T>
void FFTMagnitudeShowGnuPlot()
{
    // Same f-delta as used by the FilterSpectrumReader during sweeps
    double fdelta = (((double) Config.Rate / 2) / ((double) Config.FFTSize / 2)) / (Config.Rate > 48000 ? 1 : 10);
    if( fdelta < 1 ) {
        std::cout << "Too low fft size. Unable to show plot. (reduce blocksize)" << std::endl;
        return;
    }

    // Get reference value (smallest measureable value.
    // These do not change when using zoom, so just keep the base values for a non-zoomed fft
    // Todo: Cleanup
    int start = fdelta;
    int stop = (Config.Rate / 2);
    double ref = GetCalibratedReference<T>(fdelta, start, stop, 100, false) / 100;

    // The scaling factor based on blocksize 1024 is applied when calculating
    // the db values so we need to take this into account when analyzing a real signal.
    // Also adjust for the increased number of samples by increasing fdelta by 10 (see above)
    double factor = (double) (Config.Blocksize) / 1024;
    double scale = factor * ((1024/10)*factor);
    ref *= scale;

    // Denormalize the ref fft value to match the measured signal
    ref *= 2;

    // Plot the spectrum
    FFTMagnitudeShowGnuDBPlot<T>(ref, ref, false);
}

template <typename T>
int RunFFTMagnitudePlot()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft writer", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T>* fft;
    if( Config.FCenter > 0 ) {
        fft = new HFftOutput<T>("fft output", Config.FFTSize, Config.Average, 1, &fftWriter, new HHahnWindow<T>(), Config.Rate, Config.ZoomFactor, Config.FCenter);
    } else {
        fft = new HFftOutput<T>("fft output", Config.FFTSize, Config.Average, 1, &fftWriter, new HHahnWindow<T>(), Config.IsIq);
    }

    // Buffer for the accumulated spectrum values
    numFfts = 0;
    aggregatedMagnitudeSpectrum = new double[Config.FFTSize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", fft->Writer(), rd, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete reader
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }

    // Delete fft output
    delete fft;

    // Display the final plot
    FFTMagnitudeShowGnuPlot<T>();

    return 0;
}

template <typename T>
int RunFileConverter()
{
   // Create reader
    HReader<T>* rd;
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.OutFileFormat << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, rd, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete reader
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }

    // Delete writer
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunClickRemoval()
{
   // Create reader
    HReader<T>* rd;
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", rd, Config.Blocksize, &terminated);

    // Create clickremoval filter
    HClickRemovingFilter<T>* filter = new HClickRemovingFilter<T>("click removing filter", proc.Consumer(), Config.Blocksize);

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate, filter->Consumer());
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile, filter->Consumer());
    }
    else
    {
        std::cout << "Unknown output file format " << Config.OutFileFormat << std::endl;
        return -1;
    }

    // Run clickremoval
    proc.Run(Config.BlockCount);

    // Delete reader
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }

    // Delete writer
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }

    // delete filter
    delete filter;

    // Done
    return 0;
}

template <typename T>
int RunMixer()
{
   // Create reader 1
    HReader<T>* rd1;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd1 = new HWavReader<T>("wav reader", Config.InputFile1);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd1 = new HFileReader<T>("file reader", Config.InputFile1);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

   // Create reader 2
    HReader<T>* rd2;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd2 = new HWavReader<T>("wav reader", Config.InputFile2);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd2 = new HFileReader<T>("file reader", Config.InputFile2);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  mixer
    HLinearMixer<T> mixer("linear mixer", rd1, rd2, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) &mixer, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete readers and the writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd1;
        delete (HWavReader<T>*) rd2;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd1;
        delete (HFileReader<T>*) rd2;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunSubtracter()
{
   // Create reader 1
    HReader<T>* rd1;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd1 = new HWavReader<T>("wav reader", Config.InputFile1);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd1 = new HFileReader<T>("file reader", Config.InputFile1);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

   // Create reader 2
    HReader<T>* rd2;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd2 = new HWavReader<T>("wav reader", Config.InputFile2);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd2 = new HFileReader<T>("file reader", Config.InputFile2);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  subtracter
    HSubtracter<T> subtracter("subtracter", rd1, rd2, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) &subtracter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete readers and the writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd1;
        delete (HWavReader<T>*) rd2;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd1;
        delete (HFileReader<T>*) rd2;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunMultiplier()
{
   // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  multiplier
    if( Config.IsIq ) {
        HIqMultiplier<T> multiplier("multiplier", rd, Config.Rate, Config.Frequency, 10, Config.Blocksize);
        HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) &multiplier, Config.Blocksize, &terminated);
        proc.Run(Config.BlockCount);
    } else {
        HMultiplier<T> multiplier("multiplier", rd, Config.Rate, Config.Frequency, 10, Config.Blocksize);
        HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) &multiplier, Config.Blocksize, &terminated);
        proc.Run(Config.BlockCount);
    }

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunFilter()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HFirFilter") == 0 )
    {
        if( Config.IsIq ) {
            filter = HIqFirFilter<T>::Create("iq fir filter", (HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);
        } else {
            filter = HFirFilter<T>::Create("fir filter", (HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);
        }
    }
    else if( strcmp(Config.FilterName, "HIirFilter") == 0 )
    {
        filter = HIirFilter<T>::Create("iir filter", (HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
class FilterSpectrumReader : public HReader<T>
{
    private:

        HVfo<T>* vfo;
        int _freqStart;
        int _freqStop;
        int _freq;
        double _fDelta;
        int _lastDisplayedFreq;

        double _ref;

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }

    public:

        FilterSpectrumReader(std::string id):
            HReader<T>(id),
            _ref(0)
        {
            // Calculate f-delta
            _fDelta = (((double) Config.Rate / 2) / ((double) Config.FFTSize / 2)) / 10;

            if( _fDelta < 1 )
            {
                std::cout << "Warning: Too low fft size (reduce blocksize)" << std::endl;
                _fDelta = 1;
            }

            // Initialize initial starting and stopping frequencies
            // Todo:: Cleanup
            _freqStart = _fDelta;
            _freqStop = (Config.Rate / 2);
            _freq = _freqStart;
            _lastDisplayedFreq = _freq;

            // Create vfo
            int amplitude = std::numeric_limits<T>::max() / 2;
            vfo = new HVfo<T>("vfo", Config.Rate, _freq, amplitude, 0);

            // Calibration - measure input spectrum magnitude
            _ref = GetCalibratedReference<T>(_fDelta, _freqStart, _freqStop, amplitude);
        }

        ~FilterSpectrumReader()
        {
            delete vfo;
        }

        int ReadImpl(T* dest, size_t blocksize)
        {
            // At end of sweep ?
            if( _freq >= _freqStop )
            {
                int freq = _freq / 1000;
                std::cout << "  - " << freq << "000Hz" << std::endl;
                return 0;
            }

            // Set frequency then return signal
            vfo->SetFrequency(_freq, 0);
            vfo->Read(dest, blocksize);
            _freq += _fDelta;

            if( _freq - _lastDisplayedFreq > 1000 )
            {
                int freq = _freq / 1000;
                std::cout << "  - " << freq << "000Hz" << std::endl;
                _lastDisplayedFreq = _freq;
            }

            return blocksize;
        }

        double GetRef() {
            return _ref;
        }
};

template <typename T>
int RunFilterSpectrum()
{
   // Create reader
    FilterSpectrumReader<T> rd("spectrum reader");

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HFirFilter") == 0 )
    {
        std::cout << "coefs: " << Config.FilterCoeffs << std::endl;
        filter = HFirFilter<T>::Create("fir filter", (HReader<T>*) &rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else if( strcmp(Config.FilterName, "HIirFilter") == 0 )
    {
        filter = HIirFilter<T>::Create("iir filter", (HReader<T>*) &rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft writer", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft("fft output", Config.Blocksize, 1, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", &fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run(Config.BlockCount);

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    return 0;
}

template <typename T>
int RunBiQuadSpectrum()
{
    // Create reader
    FilterSpectrumReader<T> rd("spectrum reader");

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HLowpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowpassBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighpassBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HBandpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HBandpassBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HNotchBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HNotchBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HAllpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HAllpassBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HPeakingEQBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HPeakingEQBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HLowShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowShelvingBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighShelvingBiQuad<T>, T>("biquad filter", (HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Dump the filter coefficients
    std::vector<float> coeffs = ((HIirFilter<T>*) filter)->GetCoefficients();
    std::ofstream stream;
    stream.open(Config.OutputFile);
    stream << std::fixed;
    stream << coeffs[0] << std::endl;
    stream << coeffs[1] << std::endl;
    stream << coeffs[2] << std::endl;
    stream << coeffs[3] << std::endl;
    stream << coeffs[4] << std::endl;
    stream.close();

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft writer", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft("fft output", Config.Blocksize, 1, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", &fft, filter->Reader(), Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run(Config.BlockCount);

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    return 0;
}

template <typename T>
int RunBiQuadFilter()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HLowpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowpassBiQuad<T>, T> ("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighpassBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HBandpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HBandpassBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HNotchBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HNotchBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HAllpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HAllpassBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HPeakingEQBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HPeakingEQBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HLowShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowShelvingBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighShelvingBiQuad<T>, T>("biquad filter", (HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunKaiserBesselSpectrum()
{
    // Create reader
    FilterSpectrumReader<T> rd("spectrum reader");

    // Create  filter
    HFirFilter<T>* filter;
    if( strcmp(Config.FilterName, "HLowpassKaiserBessel") == 0 )
    {
        HLowpassKaiserBessel<T> kb(Config.FStop, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd.Reader(), kb.Calculate(), Config.Points, Config.Blocksize );
        std::cout << "Ignoring Fstart for lowpass Kaiser-Bessel filter" << std::endl;
    }
    else if( strcmp(Config.FilterName, "HHighpassKaiserBessel") == 0 )
    {
        HHighpassKaiserBessel<T> kb(Config.FStart, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd.Reader(), kb.Calculate(), Config.Points, Config.Blocksize );
        std::cout << "Ignoring Fstop for highpass Kaiser-Bessel filter" << std::endl;
    }
    else if( strcmp(Config.FilterName, "HBandpassKaiserBessel") == 0 )
    {
        HBandpassKaiserBessel<T> kb(Config.FStart, Config.FStop, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd.Reader(), kb.Calculate(), Config.Points, Config.Blocksize );
    }
    else if( strcmp(Config.FilterName, "HBandstopKaiserBessel") == 0 )
    {
        HBandstopKaiserBessel<T> kb(Config.FStart, Config.FStop, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd.Reader(), kb.Calculate(), Config.Points, Config.Blocksize );
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Dump the filter coefficients
    std::vector<float> coeffs = filter->GetCoefficients();
    std::ofstream stream;
    stream.open(Config.OutputFile);
    stream << std::fixed;
    for( int i = 0; i < coeffs.size(); i++ ) {
        stream << coeffs[i] << std::endl;
    }
    stream.close();

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft result", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft("fft output", Config.Blocksize, 1, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", &fft, filter->Reader(), Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run(Config.BlockCount);

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    return 0;
}

template <typename T>
int RunKaiserBesselFilter()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HLowpasskaiserBessel") == 0 )
    {
        HLowpassKaiserBessel<T> kb(Config.FStop, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd, kb.Calculate(), Config.Points, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighpassKaiserBessel") == 0 )
    {
        HHighpassKaiserBessel<T> kb(Config.FStart, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd, kb.Calculate(), Config.Points, Config.Blocksize );
    }
    else if( strcmp(Config.FilterName, "HBandpassKaiserBessel") == 0 )
    {
        HBandpassKaiserBessel<T> kb(Config.FStart, Config.FStop, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd, kb.Calculate(), Config.Points, Config.Blocksize );
    }
    else if( strcmp(Config.FilterName, "HBandstopKaiserBessel") == 0 )
    {
        HBandstopKaiserBessel<T> kb(Config.FStart, Config.FStop, Config.Rate, Config.Points, Config.Attenuation);
        filter = new HFirFilter<T>("fir filter", rd, kb.Calculate(), Config.Points, Config.Blocksize );
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunGain()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create gain
    HGain<T> gain("gain", rd, Config.Gain, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) &gain, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunCombSpectrum()
{
    // Create reader
    FilterSpectrumReader<T> rd("spectrum reader");

    // Create  filter
    HCombFilter<T>* filter;
    if( Config.IsHumSpectrum )
    {
        filter = new HHumFilter<T>("hum filter", &rd, Config.Rate, Config.Frequency, Config.FCutOff, Config.Blocksize);
    }
    else
    {
        filter = new HCombFilter<T>("comb filter", &rd, Config.Rate, Config.Frequency, Config.Alpha, Config.Blocksize);
    }

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft writer", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft("fft output", Config.Blocksize, 1, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", &fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run(Config.BlockCount);

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    // Delete the filter
    delete filter;

    return 0;
}

template <typename T>
int RunCombFilter()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HCombFilter<T>* filter;
    if( Config.IsHum )
    {
        filter = new HHumFilter<T>("hum filter", rd, Config.Rate, Config.Frequency, Config.FCutOff, Config.Blocksize);
    }
    else
    {
        filter = new HCombFilter<T>("comb filter", rd, Config.Rate, Config.Frequency, Config.Alpha, Config.Blocksize);
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    // Delete the filter
    delete filter;

    return 0;
}

int CurrentGoertzlBin = 0;

int GoertzelMagnitudePlotWriter(HGoertzelResult* data, size_t size)
{
    aggregatedMagnitudeSpectrum[CurrentGoertzlBin] += data->Magnitude;
    numFfts++;
    return size;
}

template <typename T>
int RunGoertzlSweep()
{
    // Create and initialize buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize];
    for( int i = 0; i < Config.Blocksize; i ++ )
    {
        aggregatedMagnitudeSpectrum[i] = 0;
    }

    // Sweep
    HRectangularWindow<T> window;
    HCustomWriter<HGoertzelResult> resultWriter("result writer", GoertzelMagnitudePlotWriter);
    HGoertzelOutput<T>* filter;
    HReader<T>* rd;
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    for( int i = 0; i < Config.Blocksize; i++ )
    {
        // Reset fft counter
        numFfts = 0;

        // Create reader
        if( strcmp(Config.FileFormat, "wav") == 0 )
        {
            rd = new HWavReader<T>("wav reader", Config.InputFile);
        }
        else if( strcmp(Config.FileFormat, "pcm") == 0 )
        {
            rd = new HFileReader<T>("file reader", Config.InputFile);
        }
        else
        {
            std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
            return -1;
        }

        // Create Goertzl filter at this bin
        filter = new HGoertzelOutput<T>("goertzel filter", Config.Blocksize, 4, i, &resultWriter, &window);

        // Create processor
        HStreamProcessor<T> proc("stream processor", filter, rd, Config.Blocksize, &terminated);
        proc.Run(Config.BlockCount);
        delete filter;

        // Delete the reader
        if( strcmp(Config.FileFormat, "wav") == 0 )
        {
            delete (HWavReader<T>*) rd;
        }
        else if( strcmp(Config.FileFormat, "pcm") == 0 )
        {
            delete (HFileReader<T>*) rd;
        }

        // Show progress
        int fDelta = (Config.Rate / Config.Blocksize) / 2;
        if( fDelta < 1 ) {
            std::cout << "Too low fftsize. (reduce blocksize)" << std::endl;
            return -1;
        }
        if( CurrentGoertzlBin % (Config.Blocksize / 10) == 0 && CurrentGoertzlBin > 0 )
        {
            std::cout << "  - " << (i * fDelta) << "Hz" << " = bin " << i << std::endl;
        }

        // Next bin
        CurrentGoertzlBin ++;
    }

    // Display the final plot
    FFTMagnitudeShowGnuPlot<T>();

    return 0;
}

template <typename T>
int RunBiQuadCascadeFilter()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HCascadedBiQuadFilter<T>* filter = HCascadedBiQuadFilter<T>::Create("biquad filter", (HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    // Delete the filter
    delete filter;

    return 0;
}

template <typename T>
int RunBiQuadCascadeSpectrum()
{
   // Create reader
    FilterSpectrumReader<T> rd("spectrum reader");

    // Create  filter
    HCascadedBiQuadFilter<T>* filter = HCascadedBiQuadFilter<T>::Create("biquad filter", (HReader<T>*) &rd, Config.Blocksize, Config.FilterCoeffs);

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft writer", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft("fft output", Config.Blocksize, 1, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", &fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run(Config.BlockCount);

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    // Delete the filter
    delete filter;

    return 0;
}

template <typename T>
int RunMovingAverageSpectrum()
{
    // Create reader
    FilterSpectrumReader<T> rd("spectrum reader");

    // Create  filter
    HMovingAverageFilter<T>* filter = new HMovingAverageFilter<T>("moving average", &rd, Config.M, Config.Blocksize);

    // Create writer
    HCustomWriter<HFftResults> fftWriter("fft result", FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft("fft output", Config.Blocksize, 1, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc("stream processor", &fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run(Config.BlockCount);

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    // Delete the filter
    delete filter;

    return 0;
}

template <typename T>
int RunMovingAverageFilter()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HMovingAverageFilter<T>* filter = new HMovingAverageFilter<T>("moving average", rd, Config.M, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, filter->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    // Delete the filter
    delete filter;

    return 0;
}

template <typename T>
int RunReal2Iq()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr = new HFileWriter<T>("file writer", Config.OutputFile);

    // Create  converter
    HReal2IqConverter<T>* converter = new HReal2IqConverter<T>("real to iq converter", rd, Config.Blocksize / 2);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, converter->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }
    delete (HFileWriter<T>*) wr;

    // Delete the converter
    delete converter;

    return 0;
}

template <typename T>
int RunIq2Real()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr = new HFileWriter<T>("file writer", Config.OutputFile);

    // Create  converter
    HIq2RealConverter<T>* converter = new HIq2RealConverter<T>("iq to real converter", rd, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, converter->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }
    delete (HFileWriter<T>*) wr;

    // Delete the converter
    delete converter;

    return 0;
}

template <typename T>
int RunIq2Abs()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr = new HFileWriter<T>("file writer", Config.OutputFile);

    // Create  converter
    HIq2AbsConverter<T>* converter = new HIq2AbsConverter<T>("iq to abs converter", rd, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, converter->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }
    delete (HFileWriter<T>*) wr;

    // Delete the converter
    delete converter;

    return 0;
}

template <typename T>
int RunFft()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<std::complex<double>>* wr;
    wr = new HFileWriter<std::complex<double>>("file writer", Config.OutputFile);

    // Start reader and writer
    rd->Start();
    wr->Start();

    // Create FFT
    HHahnWindow<T> window;
    HFft<T> fft(Config.Blocksize, &window);

    // Convert signal
    T input[Config.Blocksize];
    std::complex<double>* output = new std::complex<double>[Config.Blocksize];
    while( rd->Read(input, Config.Blocksize) == Config.Blocksize ) {
        fft.FFT(input, output);
        wr->Write(output, Config.Blocksize);
    }
    delete[] output;

    // Stop reader and writer
    rd->Stop();
    wr->Stop();

    // Delete the reader and writer
    if( strcmp(Config.InFileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }
    delete (HFileWriter<T>*) wr;

    return 0;
}

template <typename T>
int RunIfft()
{
    // Create reader
    HReader<std::complex<double>>* rd;
    rd = new HFileReader<std::complex<double>>("filereader", Config.InputFile);

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.OutFileFormat << std::endl;
        return -1;
    }

    // Start reader and writer
    rd->Start();
    wr->Start();

    // Create FFT
    HHahnWindow<T> window;
    HFft<T> fft(Config.Blocksize, &window);

    // Convert fft
    std::complex<double>* input = new std::complex<double>[Config.Blocksize];
    T output[Config.Blocksize];
    while( rd->Read(input, Config.Blocksize) == Config.Blocksize ) {
        fft.IFFT(input, output);
        wr->Write(output, Config.Blocksize);
    }
    delete[] input;

    // Stop reader and writer
    rd->Stop();
    wr->Stop();

    // Delete the reader and writer
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        delete (HFileWriter<T>*) wr;
    }
    delete (HFileReader<T>*) rd;

    return 0;
}

template <typename T>
int RunDemux()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writers
    HWriter<T>* wr1;
    HWriter<T>* wr2;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr1 = new HWavWriter<T>("wav writer", ("1_" + std::string(Config.OutputFile)).c_str(), Config.Format, 1, Config.Rate);
        wr2 = new HWavWriter<T>("wav writer", ("2_" + std::string(Config.OutputFile)).c_str(), Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr1 = new HFileWriter<T>("file writer", "1_" + std::string(Config.OutputFile));
        wr2 = new HFileWriter<T>("file writer", "2_" + std::string(Config.OutputFile));
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  demultiplexer
    std::vector<HWriter<T>*> writers;
    writers.push_back(wr1);
    writers.push_back(wr2);
    HDeMux<T> dmx("demux", writers, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", dmx.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr1;
        delete (HWavWriter<T>*) wr2;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr1;
        delete (HFileWriter<T>*) wr2;
    }

    return 0;
}

template <typename T>
int RunDecimator()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create decimator and a processor
    HDecimator<T> dcm("decimator", wr, Config.DecimateFactor, Config.Blocksize);
    HStreamProcessor<T> proc("stream processor", dcm.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunTranslator()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  decimator and a processor
    HStreamProcessor<T>* proc;
    if( Config.IsTranslateByTwo ) {
        if( Config.IsIq ) {
            HIqTranslateByTwo<T> tr2("translate by two", wr, Config.Blocksize);
            proc = new HStreamProcessor<T>("stream processor", tr2.Writer(), rd->Reader(), Config.Blocksize, &terminated);
            proc->Run();
            delete proc;
        } else {
            HTranslateByTwo<T> tr2("translate by two", wr, Config.Blocksize);
            proc = new HStreamProcessor<T>("stream processor", tr2.Writer(), rd->Reader(), Config.Blocksize, &terminated);
            proc->Run();
            delete proc;
        }
    } else if( Config.IsTranslateByFourI || Config.IsTranslateByFourQ ) {
        if( Config.IsIq ) {
            HIqTranslateByFour<T> tr4("translate by four", wr, Config.Blocksize, Config.IsTranslateByFourQ);
            proc = new HStreamProcessor<T>("stream processor", tr4.Writer(), rd->Reader(), Config.Blocksize, &terminated);
            proc->Run();
            delete proc;
        } else {
            HTranslateByFour<T> tr4("translate by four", wr, Config.Blocksize, Config.IsTranslateByFourQ);
            proc = new HStreamProcessor<T>("stream processor", tr4.Writer(), rd->Reader(), Config.Blocksize, &terminated);
            proc->Run();
            delete proc;
        }
    } else {
        std::cout << "Called RunTranslator() for an operation that is not translation!" << std::endl;
        return -1;
    }

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunUpsampler()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  upsampler
    HUpsampler<T> up("upsampler", wr, Config.UpsampleFactor, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", up.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunInterpolator()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Read filter coefficients
    std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(Config.FilterCoeffs);
    float coefficients[coeffs.size()];
    for( int i = 0; i < coeffs.size(); i++ ) {
        coefficients[i] = coeffs[i];
    }

    // Create  interpolator
    HInterpolator<T> ip("interpolator", wr, Config.UpsampleFactor, coefficients, coeffs.size(), Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", ip.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunBaseband()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create Baseband translator
    HBaseband<T> bb("baseband", wr, Config.Rate, Config.FCenter, Config.Width, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc("stream processor", bb.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    return 0;
}

template <typename T>
int RunFirDecimator()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>("wav reader", Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>("file reader", Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>("wav writer", Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>("file writer", Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HReader<T>* filter;
    HFirDecimator<T>* firDecimator = nullptr;
    HIqFirDecimator<T>* iqFirDecimator = nullptr;
    std::vector<float> coefs = HFilter<T>::ReadCoeffsFromFile(Config.FilterCoeffs);
    if( Config.IsIq ) {
        iqFirDecimator = new HIqFirDecimator<T>("iq fir decimator", rd->Reader(), Config.DecimateFactor, &coefs[0], coefs.size(), Config.Blocksize);
        filter = iqFirDecimator->Reader();
    } else {
        firDecimator = new HFirDecimator<T>("fir decimator", rd->Reader(), Config.DecimateFactor, &coefs[0], coefs.size(), Config.Blocksize);
        filter = firDecimator->Reader();
    }

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    // Delete the reader and writer
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
        delete (HWavWriter<T>*) wr;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
        delete (HFileWriter<T>*) wr;
    }

    // Delete the filter
    if( firDecimator != nullptr ) {
        delete firDecimator;
    } else {
        delete iqFirDecimator;
    }

    return 0;
}

template <typename T>
int RunSampleTypeConverter()
{
    // Create reader
    HFileReader<float> rd("file reader", Config.InputFile);

    // Create converter
    HTypeConverter<float, T> cnv("type converter", rd.Reader(), Config.Blocksize, Config.TypeConverterScale);

    // Create writer
    HFileWriter<T> wr("file writer", Config.OutputFile);

    // Create processor
    HStreamProcessor<T> proc("stream processor", wr.Writer(), cnv.Reader(), Config.Blocksize, &terminated);
    proc.Run(Config.BlockCount);

    return 0;
}

template <typename T>
int RunOperation()
{
    // Wait for start time ?
    if( Config.Timer.Duration() > 0 )
    {
        time_t stamp = Config.Timer.Start();
        struct tm* timeInfo = localtime( &stamp );
        std::cout << "Operation starts at " << asctime(timeInfo) << std::endl;
        Config.Timer.Wait();
        stamp = Config.Timer.Stop();
        timeInfo = localtime( &stamp );
        std::cout << "Operation stops at " << asctime(timeInfo) << std::endl;
    }

    // Show audio device information
    if( Config.ShowAudioDevices ) {

        if( HSoundcard::AvailableDevices() == 0 )
        {
            std::cout << "There is no soundcards available on this system" << std::endl;
            return 0;
        }

        std::cout << "Default device is: " << HSoundcard::GetDefaultDevice() << std::endl;
        std::cout << std::endl;

        std::vector<HSoundcard::DeviceInformation> info = HSoundcard::GetDeviceInformation();
        for( std::vector<HSoundcard::DeviceInformation>::iterator it = info.begin(); it != info.end(); it++)
        {
            std::cout << "Device: " << (*it).Device << std::endl;
            std::cout << "Device: " << (*it).Name << std::endl;
            std::cout << "Inputs: " << (*it).Inputs << std::endl;
            std::cout << "Outputs: " << (*it).Outputs << std::endl;
            std::cout << "Is default device: " << ((*it).IsDefaultDevice ? "yes" : "no") << std::endl;
            std::cout << std::endl;
        }
        return 0;
    }


    // Show RTL2832 device information
    if( Config.ShowRtl2832Devices ) {

        if( HRtl2832::AvailableDevices() == 0 )
        {
            std::cout << "There is no RTL2832 devices available on this system" << std::endl;
            return 0;
        }

        std::cout << "Default device is: " << HRtl2832::GetDefaultDevice() << std::endl;
        std::cout << std::endl;

        std::vector<HRtl2832::DeviceInformation> info = HRtl2832::GetDeviceInformation();
        for( std::vector<HRtl2832::DeviceInformation>::iterator it = info.begin(); it != info.end(); it++)
        {
            std::cout << "Device: " << (*it).Device << std::endl;
            std::cout << "Vendor: " << (*it).Vendor << std::endl;
            std::cout << "Product: " << (*it).Product << std::endl;
            std::cout << "Serial: " << (*it).Serial << std::endl;
            std::cout << "Is default device: " << ((*it).IsDefaultDevice ? "yes" : "no") << std::endl;
            std::cout << "Available gain values:" << std::endl;
            for( std::vector<int>::iterator git = (*it).Gain.begin(); git != (*it).Gain.end(); git++ ) {
                std::cout << "                       " << std::to_string(*git) << std::endl;
            }
            std::cout << std::endl;
        }
        return 0;
    }

    // Read from network and write to local file
    if( Config.IsNetworkReaderClient )
    {
        // Verify configuration
        if( Config.Address == NULL )
        {
            std::cout << "No Server address (-nr server port)" << std::endl;
            return 1;
        }
        if( Config.Port == -1 )
        {
            std::cout << "No Server port (-nr server port)" << std::endl;
            return 1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return true;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No output file format (-ff)" << std::endl;
            return true;
        }

        return RunNetworkReaderClient<T>();
    }

    // Read from soundcard and write to network
    if( Config.IsNetworkWriterServer)
    {
        // Verify configuration
        if( Config.InputDevice == -1 )
        {
            std::cout << "No inputdevice (-id)" << std::endl;
            return 1;
        }
        if( Config.Port == -1 )
        {
            std::cout << "No Server port (-ns port)" << std::endl;
            return 1;
        }

        return RunNetworkWriterServer<T>();
    }

    // Generator a single tone
    if( Config.IsSignalGenerator )
    {
        // Verify configuration
        if( Config.Frequency == 0 )
        {
            std::cout << "Frequency is 0, not possible" << std::endl;
            return 1;
        }
        if( Config.OutputFile == NULL && Config.OutputDevice == -1 )
        {
            std::cout << "No output file or device (-of|-od)" << std::endl;
            return true;
        }
        if( Config.OutputFile != NULL && Config.FileFormat == NULL )
        {
            std::cout << "No output file format (-ff)" << std::endl;
            return true;
        }

        return RunSignalGenerator<T>();
    }

    // Record to a local file
    if( Config.IsFileRecorder )
    {
        // Verify configuration
        if( Config.InputDevice == -1 )
        {
            std::cout << "No input device (-id)" << std::endl;
            return -1;
        }
        if( Config.InputDeviceType == DspCmdConfig::DeviceType::NONE )
        {
            std::cout << "No input device type (-it)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No output file format (-ff)" << std::endl;
            return -1;
        }

        return RunFileRecorder<T>();
    }

    // Play a local file
    if( Config.IsFilePlayer )
    {
        // Verify configuration
        if( Config.OutputDevice == -1 )
        {
            std::cout << "No output device (-od)" << std::endl;
            return -1;
        }
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff)" << std::endl;
            return -1;
        }

        return RunFilePlayer<T>();
    }

    // Run an FFT on a local file and either write or plot the magnitude spectrum
    if( Config.IsFFTMagnitudeGnuPlot )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff)" << std::endl;
            return -1;
        }
        if( Config.FFTSize == 0 || (Config.FFTSize % 2) > 0 )
        {
            std::cout << "Invalid fft size" << std::endl;
            return -1;
        }
        if( Config.Average < 1 )
        {
            std::cout << "Average is too small. At least 1 fft must be taken" << std::endl;
            return -1;
        }

        // Adjust blocksize
        if( Config.FFTSize > Config.Blocksize )
        {
            Config.Blocksize = Config.FFTSize;
            std::cout << "Blocksize < FFT size, setting new blocksize " << Config.Blocksize << std::endl;
        }

        return RunFFTMagnitudePlot<T>();
    }

    if( Config.IsFileConverter )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.InFileFormat == NULL )
        {
            std::cout << "No input file format (-fc in-format out-format)" << std::endl;
            return -1;
        }
        if( Config.OutFileFormat == NULL )
        {
            std::cout << "No output file format (-fc in-format out-format)" << std::endl;
            return -1;
        }

        return RunFileConverter<T>();
    }

    if( Config.IsClickRemoval )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.InFileFormat == NULL )
        {
            std::cout << "No input file format (-cr in-format out-format)" << std::endl;
            return -1;
        }
        if( Config.OutFileFormat == NULL )
        {
            std::cout << "No output file format (-cr in-format out-format)" << std::endl;
            return -1;
        }

        return RunClickRemoval<T>();
    }

    if( Config.IsClickRemoval )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.InFileFormat == NULL )
        {
            std::cout << "No input file format (-cr in-format out-format)" << std::endl;
            return -1;
        }
        if( Config.OutFileFormat == NULL )
        {
            std::cout << "No output file format (-cr in-format out-format)" << std::endl;
            return -1;
        }

        return RunClickRemoval<T>();
    }

    if( Config.IsMixer )
    {
        // Verify configuration
        if( Config.InputFile1 == NULL )
        {
            std::cout << "No input file 1 (-mx file1 file2)" << std::endl;
            return -1;
        }
        if( Config.InputFile2 == NULL )
        {
            std::cout << "No input file 2 (-mx file1 file2)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff pcm|wav)" << std::endl;
            return -1;
        }

        return RunMixer<T>();
    }

    if( Config.IsSubtracter )
    {
        // Verify configuration
        if( Config.InputFile1 == NULL )
        {
            std::cout << "No input file 1 (-sb file1 file2)" << std::endl;
            return -1;
        }
        if( Config.InputFile2 == NULL )
        {
            std::cout << "No input file 2 (-sb file1 file2)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff pcm|wav)" << std::endl;
            return -1;
        }

        return RunSubtracter<T>();
    }

    if( Config.IsMultiplier )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if file)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff pcm|wav)" << std::endl;
            return -1;
        }

        return RunMultiplier<T>();
    }

    if( Config.IsFilter )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if file)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff pcm|wav)" << std::endl;
            return -1;
        }
        if( Config.FilterName == NULL )
        {
            std::cout << "No filter name (-flt name coeffs)" << std::endl;
            return -1;
        }
        if( Config.FilterCoeffs == NULL )
        {
            std::cout << "No filter coeffs. filename (-flt name coeffs)" << std::endl;
            return -1;
        }

        return RunFilter<T>();
    }

    if( Config.IsFilterSpectrumGnuPlot)
    {
        if( Config.FilterName == NULL )
        {
            std::cout << "No filter name (-flt name coeffs)" << std::endl;
            return -1;
        }
        if( Config.FilterCoeffs == NULL )
        {
            std::cout << "No filter coeffs. filename (-flt name coeffs)" << std::endl;
            return -1;
        }

        return RunFilterSpectrum<T>();
    }

    if( Config.IsBiQuadSpectrum )
    {
        if( Config.FilterName == NULL )
        {
            std::cout << "No filter name (-bq name Fcutoff Q G)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file, for the coefficients (-of)" << std::endl;
            return -1;
        }

        return RunBiQuadSpectrum<T>();
    }

    if( Config.IsBiQuad )
    {
        if( Config.FilterName == NULL )
        {
            std::cout << "No filter name (-bq name Fcutoff Q G)" << std::endl;
            return -1;
        }
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunBiQuadFilter<T>();
    }

    if( Config.IsKaiserBesselSpectrum )
    {
        if( Config.FilterName == NULL )
        {
            std::cout << "No filter name (-kb name start stop att)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file, for the coefficients (-of)" << std::endl;
            return -1;
        }

        return RunKaiserBesselSpectrum<T>();
    }

    if( Config.IsKaiserBessel )
    {
        if( Config.FilterName == NULL )
        {
            std::cout << "No filter name (-kbt name start stop att)" << std::endl;
            return -1;
        }
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunKaiserBesselFilter<T>();
    }

    if( Config.IsGain )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunGain<T>();
    }

    if( Config.IsCombSpectrum )
    {
        if( Config.Alpha == 0 )
        {
            std::cout << "Alpha can not be 0 (zero) (-cb fBase alpha)" << std::endl;
            return -1;
        }
        return RunCombSpectrum<T>();
    }

    if( Config.IsComb )
    {
        if( Config.Alpha == 0 )
        {
            std::cout << "Alpha can not be 0 (zero) (-cb fBase alpha)" << std::endl;
            return -1;
        }
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunCombFilter<T>();
    }

    if( Config.IsHumSpectrum )
    {
        return RunCombSpectrum<T>();
    }

    if( Config.IsHum )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunCombFilter<T>();
    }

    if( Config.IsGoertzl )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        return RunGoertzlSweep<T>();
    }

    if( Config.IsBiQuadCascade )
    {
        // Verify configuration
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if file)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.FileFormat == NULL )
        {
            std::cout << "No input file format (-ff pcm|wav)" << std::endl;
            return -1;
        }
        if( Config.FilterCoeffs == NULL )
        {
            std::cout << "No filter coeffs. filename (-gqct name coeffs)" << std::endl;
            return -1;
        }

        return RunBiQuadCascadeFilter<T>();
    }

    if( Config.IsBiQuadCascadeSpectrum)
    {
        if( Config.FilterCoeffs == NULL )
        {
            std::cout << "No filter coeffs. filename (-bgc coeffs)" << std::endl;
            return -1;
        }

        return RunBiQuadCascadeSpectrum<T>();
    }

    if( Config.IsMovingAverageSpectrum )
    {
        if( Config.M == 0 )
        {
            std::cout << "Moving average factor M not set (-mat M)" << std::endl;
            return -1;
        }
        
        return RunMovingAverageSpectrum<T>();
    }

    if( Config.IsMovingAverage )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }
        if( Config.M == 0 )
        {
            std::cout << "Moving average factor M not set (-mat M)" << std::endl;
            return -1;
        }

        return RunMovingAverageFilter<T>();
    }

    if( Config.IsReal2Iq )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunReal2Iq<T>();
    }

    if( Config.IsIq2Real )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunIq2Real<T>();
    }

    if( Config.IsIq2Abs )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunIq2Abs<T>();
    }

    if( Config.IsFft )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }
        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunFft<T>();
    }

    if( Config.IsIfft )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunIfft<T>();
    }

    if( Config.IsDemux )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunDemux<T>();
    }

    if( Config.IsDecimator )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunDecimator<T>();
    }

    if( Config.IsTranslateByTwo || Config.IsTranslateByFourI || Config.IsTranslateByFourQ )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunTranslator<T>();
    }

    if( Config.IsUpsampler )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        return RunUpsampler<T>();
    }

    if( Config.IsInterpolator )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        if( Config.FilterCoeffs == NULL )
        {
            std::cout << "No filter coeffs. filename (-ip factor coeffs)" << std::endl;
            return -1;
        }

        return RunInterpolator<T>();
    }

    if( Config.IsBaseband )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        if( Config.FCenter == 0 )
        {
            std::cout << "No center frequency (-bb CENTER ...)" << std::endl;
            return -1;
        }

        if( Config.Width == 0 )
        {
            std::cout << "No width (-bb ... WIDTH)" << std::endl;
            return -1;
        }

        if( (Config.Rate / 2) % Config.Width != 0 ) {
            std::cout << "Illegal width A" << std::endl;
            return -1;
        }

        if( Config.Width > (Config.Rate / 2) /2 ) {
            std::cout << "Illegal width B" << std::endl;
            return -1;
        }

        if( Config.FCenter - (Config.Width / 2) <= 0 || Config.FCenter + (Config.Width / 2) >= (Config.Rate / 2 ) ) {
            std::cout << "Illegal center frequency" << std::endl;
            return -1;
        }

        return RunBaseband<T>();
    }

    if( Config.IsFirDecimator )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        if( Config.FilterCoeffs == NULL )
        {
            std::cout << "No filter coeffs. filename (-fdcm coeffs)" << std::endl;
            return -1;
        }

        return RunFirDecimator<T>();
    }

    if( Config.IsSampleTypeConverter )
    {
        if( Config.InputFile == NULL )
        {
            std::cout << "No input file (-if)" << std::endl;
            return -1;
        }

        if( Config.OutputFile == NULL )
        {
            std::cout << "No output file (-of)" << std::endl;
            return -1;
        }

        if( Config.SampleInType == 0 )
        {
            std::cout << "No input sample type" << std::endl;
            return -1;
        }

        if( Config.SampleOutType == 0 )
        {
            std::cout << "No output sample type" << std::endl;
            return -1;
        }

        return RunSampleTypeConverter<T>();
    }

    // No known operation could be determined from the input arguments
    std::cout << "Sorry, operation could not be determined from input arguments" << std::endl;
    std::cout << "Try 'dspcmd -h'" << std::endl;
	return 1;
}

int RunOperation()
{
    // Two stage operation calling, first find out which datatype is needed
    try
    {
        int rc;
        switch(Config.Format)
        {
            case H_SAMPLE_FORMAT_INT_8:
                HLog("Base datatype is int8_t");
                return RunOperation<int8_t>();
            case H_SAMPLE_FORMAT_UINT_8:
                HLog("Base datatype is uint8_t");
                return RunOperation<uint8_t>();
            case H_SAMPLE_FORMAT_INT_16:
                HLog("Base datatype is int16_t");
                return RunOperation<int16_t>();
            /*case H_SAMPLE_FORMAT_INT_24:
                HLog("Base datatype is int24_t");
                return RunOperation<int8_t>();*/
            case H_SAMPLE_FORMAT_INT_32:
                HLog("Base datatype is int32_t");
                return RunOperation<int32_t>();
            default:
                std::cout << "Unknown sample format " << Config.Format << std::endl;
                return -1;
        }
    }
    catch( HException* hex )
    {
        std::cout << "Caught exception:" << std::endl;
        std::cout << "  " << hex->what() << std::endl;
        return 1;
    }
    catch( std::exception* ex )
    {
        std::cout << "Caught external exception:" << std::endl;
        std::cout << "  " << ex->what() << std::endl;
        return 2;
    }
    catch( ... )
    {
        std::cout << "Unexpected exception, no further information" << std::endl;
        return 3;
    }
}
