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
    HVfo<T> vfo(Config.Rate, freq, amplitude, 0);

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
    HSoundcardReader<T> rdr(Config.InputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    HNetworkProcessor<T> srv(Config.Port, Config.Port + 1, &rdr, Config.Blocksize, &terminated);
    try
    {
        srv.Run();
        std::cout << rdr.GetMetrics("HSoundcardReader");
        std::cout << srv.GetMetrics("HNetworkProcessor");
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create a network processor and run the client
    HNetworkProcessor<T> client(Config.Address, Config.Port, Config.Port + 1, wr, Config.Blocksize, &terminated);
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
        wr = new HSoundcardWriter<T>(Config.OutputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    }
    else if ( Config.OutputFile != NULL )
    {
        if( strcmp(Config.FileFormat, "wav") == 0 )
        {
            wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
        }
        else if( strcmp(Config.FileFormat, "pcm") == 0 )
        {
            wr = new HFileWriter<T>(Config.OutputFile);
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
    HVfo<T> sg(Config.Rate, Config.Frequency, Config.Amplitude, Config.Phase);
    HStreamProcessor<T> proc(wr, &sg, Config.Blocksize, &terminated);
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
    HSoundcardReader<T> rd(Config.InputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }


    // Create processor
    HStreamProcessor<T> proc(wr, &rd, Config.Blocksize, &terminated);
    if( Config.Timer.Duration() > 0 )
    {
        unsigned long int blocks = (Config.Timer.Duration() * Config.Rate) / Config.Blocksize;
        proc.Run(blocks);
    }
    else
    {
        proc.Run();
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
int RunFilePlayer()
{
    // Create reader
    HReader<T>* rd;
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HSoundcardWriter<T> wr(Config.OutputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(&wr, rd, Config.Blocksize, &terminated);
    proc.Run();

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

    // Calculate frequency span per bin
    double fdelta = ((double) Config.Rate / 2) / ((double)Config.FFTSize / 2);

    // Plot lines
    FILE* gnuplotPipe = popen ("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2 pointtype -1\n");
    fprintf(gnuplotPipe, "set xlabel \"Hz\" offset 0,0\n");
    fprintf(gnuplotPipe, "set ylabel \"DB\" offset 0,0\n");
    if( Config.XMax > 0 )
    {
        fprintf(gnuplotPipe, "set xrange [%d:%d]\n",Config.XMin, Config.XMax);
    }
    if( Config.YMax > 0 )
    {
        fprintf(gnuplotPipe, "set yrange [%d:%d]\n", Config.YMin, Config.YMax);
    }
    fprintf(gnuplotPipe, "plot '-' with linespoints linestyle 1 title \"%d point fft\"\n", Config.Blocksize);
    for( int bin = 3; bin < (Config.FFTSize / 2) - 3; bin++ )
    {
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
            fprintf(gnuplotPipe, "%lf %lf\n", (double) bin * fdelta, db);
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
    double fdelta = (((double) Config.Rate / 2) / ((double) Config.FFTSize / 2)) / 10;

    // Get reference value (smallest measureable value
    int start = Config.XMin == 0 ? fdelta : Config.XMin;
    int stop = (Config.XMax == 0 ? (Config.Rate / 2) : Config.XMax);
    double ref = GetCalibratedReference<T>(fdelta, start, stop, 1, false);

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
    }
    else
    {
        std::cout << "Unknown file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft(Config.FFTSize, Config.Average, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    numFfts = 0;
    aggregatedMagnitudeSpectrum = new double[Config.FFTSize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, rd, Config.Blocksize, &terminated);
    proc.Run();

    // Delete reader
    if( strcmp(Config.FileFormat, "wav") == 0 )
    {
        delete (HWavReader<T>*) rd;
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        delete (HFileReader<T>*) rd;
    }

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.OutFileFormat << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc(wr, rd, Config.Blocksize, &terminated);
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc(rd, Config.Blocksize, &terminated);

    // Create clickremoval filter
    HClickRemovingFilter<T>* filter = new HClickRemovingFilter<T>(proc.Consumer(), Config.Blocksize);

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate, filter->Consumer());
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile, filter->Consumer());
    }
    else
    {
        std::cout << "Unknown output file format " << Config.OutFileFormat << std::endl;
        return -1;
    }

    // Run clickremoval
    proc.Run();

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
        rd1 = new HWavReader<T>(Config.InputFile1);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd1 = new HFileReader<T>(Config.InputFile1);
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
        rd2 = new HWavReader<T>(Config.InputFile2);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd2 = new HFileReader<T>(Config.InputFile2);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  mixer
    HLinearMixer<T> mixer(rd1, rd2, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) &mixer, Config.Blocksize, &terminated);
    proc.Run();

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
        rd1 = new HWavReader<T>(Config.InputFile1);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd1 = new HFileReader<T>(Config.InputFile1);
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
        rd2 = new HWavReader<T>(Config.InputFile2);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd2 = new HFileReader<T>(Config.InputFile2);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  subtracter
    HSubtracter<T> subtracter(rd1, rd2, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) &subtracter, Config.Blocksize, &terminated);
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  multiplier
    HMultiplier<T> multiplier(rd, Config.Rate, Config.Frequency, 10, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) &multiplier, Config.Blocksize, &terminated);
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
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
        filter = HFirFilter<T>::Create((HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else if( strcmp(Config.FilterName, "HIirFilter") == 0 )
    {
        filter = HIirFilter<T>::Create((HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run();

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

        FilterSpectrumReader():
            _ref(0)
        {
            // Calculate f-delta
            _fDelta = (((double) Config.Rate / 2) / ((double) Config.FFTSize / 2)) / 10;

            if( _fDelta < 1 )
            {
                std::cout << "Too low fft size" << std::endl;
                _fDelta = 1;
            }

            // Initialize initial starting and stopping frequencies
            _freqStart = Config.XMin == 0 ? _fDelta : Config.XMin;
            _freqStop = (Config.XMax == 0 ? (Config.Rate / 2) : Config.XMax);
            _freq = _freqStart;
            _lastDisplayedFreq = _freq;

            // Create vfo
            int amplitude = std::numeric_limits<T>::max() / 2;
            vfo = new HVfo<T>(Config.Rate, _freq, amplitude, 0);

            // Calibration - measure input spectrum magnitude
            _ref = GetCalibratedReference<T>(_fDelta, _freqStart, _freqStop, amplitude);
        }

        ~FilterSpectrumReader()
        {
            delete vfo;
        }

        int Read(T* dest, size_t blocksize)
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
    FilterSpectrumReader<T> rd;

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HFirFilter") == 0 )
    {
        filter = HFirFilter<T>::Create((HReader<T>*) &rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else if( strcmp(Config.FilterName, "HIirFilter") == 0 )
    {
        filter = HIirFilter<T>::Create((HReader<T>*) &rd, Config.Blocksize, Config.FilterCoeffs);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

    // Display the final plot
    FFTMagnitudeShowGnuDBPlot<T>(rd.GetRef());

    return 0;
}

template <typename T>
int RunBiQuadSpectrum()
{
    // Create reader
    FilterSpectrumReader<T> rd;

    // Create  filter
    HFilter<T>* filter;
    if( strcmp(Config.FilterName, "HLowpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowpassBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HBandpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HBandpassBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HNotchBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HNotchBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HAllpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HAllpassBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HPeakingEQBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HPeakingEQBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HLowShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowShelvingBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighShelvingBiQuad<T>, T>((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
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
    stream << coeffs[0] << std::endl;
    stream << coeffs[1] << std::endl;
    stream << coeffs[2] << std::endl;
    stream << coeffs[3] << std::endl;
    stream << coeffs[4] << std::endl;
    stream.close();

    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, filter->Reader(), Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
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
        filter = new HBiQuadFilter<HLowpassBiQuad<T>, T> ((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighpassBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HBandpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HBandpassBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HNotchBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HNotchBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HAllpassBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HAllpassBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HPeakingEQBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HPeakingEQBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HLowShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HLowShelvingBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else if( strcmp(Config.FilterName, "HHighShelvingBiQuad") == 0 )
    {
        filter = new HBiQuadFilter<HHighShelvingBiQuad<T>, T>((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
    }
    else
    {
        std::cout << "Unknown filtername " << Config.FilterName << std::endl;
        return -1;
    }

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create gain
    HGain<T> gain(rd, Config.Gain, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) &gain, Config.Blocksize, &terminated);
    proc.Run();

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
    FilterSpectrumReader<T> rd;

    // Create  filter
    HCombFilter<T>* filter;
    if( Config.IsHumSpectrum )
    {
        filter = new HHumFilter<T>(&rd, Config.Rate, Config.Frequency, Config.FCutOff, Config.Blocksize);
    }
    else
    {
        filter = new HCombFilter<T>(&rd, Config.Rate, Config.Frequency, Config.Alpha, Config.Blocksize);
    }

    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
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
        filter = new HHumFilter<T>(rd, Config.Rate, Config.Frequency, Config.FCutOff, Config.Blocksize);
    }
    else
    {
        filter = new HCombFilter<T>(rd, Config.Rate, Config.Frequency, Config.Alpha, Config.Blocksize);
    }

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run();

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
    HCustomWriter<HGoertzelResult> resultWriter(GoertzelMagnitudePlotWriter);
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
            rd = new HWavReader<T>(Config.InputFile);
        }
        else if( strcmp(Config.FileFormat, "pcm") == 0 )
        {
            rd = new HFileReader<T>(Config.InputFile);
        }
        else
        {
            std::cout << "Unknown input file format " << Config.FileFormat << std::endl;
            return -1;
        }

        // Create Goertzl filter at this bin
        filter = new HGoertzelOutput<T>(Config.Blocksize, 4, i, &resultWriter, &window);

        // Create processor
        HStreamProcessor<T> proc(filter, rd, Config.Blocksize, &terminated);
        proc.Run();
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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HCascadedBiQuadFilter<T>* filter = HCascadedBiQuadFilter<T>::Create((HReader<T>*) rd, Config.Blocksize, Config.FilterCoeffs);

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) filter, Config.Blocksize, &terminated);
    proc.Run();

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
    FilterSpectrumReader<T> rd;

    // Create  filter
    HCascadedBiQuadFilter<T>* filter = HCascadedBiQuadFilter<T>::Create((HReader<T>*) &rd, Config.Blocksize, Config.FilterCoeffs);

    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

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
    FilterSpectrumReader<T> rd;

    // Create  filter
    HMovingAverageFilter<T>* filter = new HMovingAverageFilter<T>(&rd, Config.M, Config.Blocksize);
    
    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFftOutput<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  filter
    HMovingAverageFilter<T>* filter = new HMovingAverageFilter<T>(rd, Config.M, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(wr, filter->Reader(), Config.Blocksize, &terminated);
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<T>* wr = new HFileWriter<T>(Config.OutputFile);

    // Create  converter
    HReal2IqConverter<T>* converter = new HReal2IqConverter<T>(rd, Config.Rate, Config.Blocksize / 2);

    // Create processor
    HStreamProcessor<T> proc(wr, converter->Reader(), Config.Blocksize, &terminated);
    proc.Run();

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

    // Delete the filter
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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.InFileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
    }
    else
    {
        std::cout << "Unknown input file format " << Config.InFileFormat << std::endl;
        return -1;
    }

    // Create writer
    HWriter<std::complex<double>>* wr;
    wr = new HFileWriter<std::complex<double>>(Config.OutputFile);

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
    rd = new HFileReader<std::complex<double>>(Config.InputFile);

    // Create writer
    HWriter<T>* wr;
    if( strcmp(Config.OutFileFormat, "wav") == 0 )
    {
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.OutFileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr1 = new HWavWriter<T>(("1_" + std::string(Config.OutputFile)).c_str(), Config.Format, 1, Config.Rate);
        wr2 = new HWavWriter<T>(("2_" + std::string(Config.OutputFile)).c_str(), Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr1 = new HFileWriter<T>("1_" + std::string(Config.OutputFile));
        wr2 = new HFileWriter<T>("2_" + std::string(Config.OutputFile));
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
    HDeMux<T> dmx(writers, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(dmx.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run();

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
        rd = new HWavReader<T>(Config.InputFile);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        rd = new HFileReader<T>(Config.InputFile);
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
        wr = new HWavWriter<T>(Config.OutputFile, Config.Format, 1, Config.Rate);
    }
    else if( strcmp(Config.FileFormat, "pcm") == 0 )
    {
        wr = new HFileWriter<T>(Config.OutputFile);
    }
    else
    {
        std::cout << "Unknown output file format " << Config.FileFormat << std::endl;
        return -1;
    }

    // Create  decimator
    HDecimator<T> dcm(wr, Config.DecimateFactor, Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(dcm.Writer(), rd->Reader(), Config.Blocksize, &terminated);
    proc.Run();

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
