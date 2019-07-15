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

template <typename T>
int RunNetworkWriterServer()
{
    HSoundcardReader<T> rdr(Config.InputDevice, Config.Rate, 1, Config.Format, Config.Blocksize);
    HNetworkProcessor<T> srv(Config.Port, &rdr, Config.Blocksize, &terminated);
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
    HNetworkProcessor<T> client(Config.Address, Config.Port, wr, Config.Blocksize, &terminated);
    try
    {
        if( Config.Timer.duration() > 0 )
        {
            unsigned long int blocks = (Config.Timer.duration() * Config.Rate) / Config.Blocksize;
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

    // Calculate how many blocks fit into 10 seconds
    unsigned long int blocks = (Config.Duration * Config.Rate) / Config.Blocksize;

    // Create and run the signalgenerator
    HVfo<T> sg(Config.Rate, Config.Frequency, (T) 5000, Config.Phase);
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
    if( Config.Timer.duration() > 0 )
    {
        unsigned long int blocks = (Config.Timer.duration() * Config.Rate) / Config.Blocksize;
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

void FFTMagnitudeShowPlot()
{
    const int spectrumWidth = 80;
    const int spectrumHeight = 20;
    const int scaleWidth = 7;

    // Calculate initial averaged values
    for( int i = 0; i < Config.FFTSize / 2; i++ )
    {
        aggregatedMagnitudeSpectrum[i] = aggregatedMagnitudeSpectrum[i] / numFfts;
    }

    // Calculate how many individual spectrum lines needs to be combined per line in the spectrum graph
    int binsPerLine = (Config.FFTSize / 2) / spectrumWidth;

    // Calculate the output spectrum
    double displayedSpectrum[spectrumWidth];
    memset((void*) displayedSpectrum, 0, spectrumWidth * sizeof(double));
    int lineNo = 0;
    for( int i = 0; i < spectrumWidth; i++ )
    {
        for( int j = 0; j < binsPerLine; j++ )
        {
            displayedSpectrum[i] += aggregatedMagnitudeSpectrum[(i * binsPerLine) + j];
        }
    }

    // Calculate frequency span per bin
    int fdelta = (Config.Rate / 2) / (Config.FFTSize / 2);

    // Get maximum fft value for scaling
    int max = 0;
    for( int i = 0; i < spectrumWidth; i++ )
    {
        max = displayedSpectrum[i] > max ? displayedSpectrum[i] : max;
    }

    // Calculate how long the spectrum should be to include enough space for the last frequency label
    int plotWidth = ((spectrumWidth + scaleWidth) / scaleWidth) * scaleWidth;

    // Upper separator
    for( int col = 0; col < plotWidth; col++ )
    {
        std::cout << "-";
    }

    // Plot lines
    int scaleFactor = max / spectrumHeight;
    std::cout << std::endl;
    for( int row = 0; row < spectrumHeight; row++ )
    {
        for( int col = 0; col < spectrumWidth; col++ )
        {
            int value = displayedSpectrum[col];
            if( value > (spectrumHeight - row - 1) * scaleFactor )
            {
                std::cout << "#";
            }
            else
            {
                std::cout << (col % scaleWidth == 0 ? "." : " ");
            }
        }
        std::cout << std::endl;
    }

    // Add bottom scale separator
    for( int col = 0; col < plotWidth; col++ )
    {
        std::cout << (col % scaleWidth == 0 ? "|" : "-");
    }
    std::cout << std::endl;

    // Add frequency scale
    int fcoldelta = Config.Rate / spectrumWidth;
    for( int col = 0; col < spectrumWidth; col++ )
    {
        if( col % scaleWidth == 0 )
        {
            std::string colName = std::to_string(col * fcoldelta / 2);
            std::string padding(scaleWidth - colName.length(), ' ');
            std::cout << colName << padding;
        }
    }
    std::cout << std::endl;
}

void FFTMagnitudeShowGnuPlot()
{
    // Calculate the output spectrum
    double displayedSpectrum[Config.FFTSize / 2];
    for( int i = 0; i < Config.FFTSize / 2; i++ )
    {
        displayedSpectrum[i] = (double) aggregatedMagnitudeSpectrum[i] / numFfts;
    }

    // Calculate frequency span per bin
    double fdelta = ((double) Config.Rate / 2) / ((double)Config.FFTSize / 2);

    // Plot lines
    FILE* gnuplotPipe = popen ("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2 pointtype -1\n");
    if( Config.XMax > 0 )
    {
        fprintf(gnuplotPipe, "set xrange [%d:%d]\n",Config.XMin, Config.XMax);
    }
    if( Config.YMax > 0 )
    {
        fprintf(gnuplotPipe, "set yrange [%d:%d]\n", Config.YMin, Config.YMax);
    }
    fprintf(gnuplotPipe, "plot '-' with linespoints linestyle 1\n");
    for( int bin = 0; bin < Config.FFTSize / 2; bin++ )
    {
        fprintf(gnuplotPipe, "%lf %lf\n", (double) bin * fdelta, displayedSpectrum[bin]);
    }
    fprintf(gnuplotPipe, "e");
    fclose(gnuplotPipe);
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
    HFft<T> fft(Config.FFTSize, 4, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
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
    Config.IsFFTMagnitudePlot ? FFTMagnitudeShowPlot() : FFTMagnitudeShowGnuPlot();

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
    HMultiplier<T> multiplier(rd, Config.Rate, 1000, Config.Blocksize);

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
        int _freq;
        double _fdelta;
        int _lastDisplayedFreq;

    public:

        FilterSpectrumReader(int delta = 0)
        {
            if( delta == 0 )
            {
                _fdelta = (((double) Config.Rate / 2) / ((double) Config.FFTSize / 2)) / 10;
            }
            else
            {
                _fdelta = delta;
            }

            if( _fdelta < 1 )
            {
                std::cout << "Too low fft size" << std::endl;
                _fdelta = 1;
            }
            _freq = Config.XMin == 0 ? _fdelta : Config.XMin;
            _lastDisplayedFreq = _freq;

            vfo = new HVfo<T>(Config.Rate, _freq, std::numeric_limits<T>::max() / 2, 0);
        }

        ~FilterSpectrumReader()
        {
            delete vfo;
        }

        int Read(T* dest, size_t blocksize)
        {
            // At end of sweep ?
            if( _freq >= (Config.XMax == 0 ? (Config.Rate / 2) : Config.XMax) )
            {
                return 0;
            }

            // Set frequency then return signal
            vfo->SetFrequency(_freq, 0);
            vfo->Read(dest, blocksize);
            _freq += _fdelta;

            if( _freq - _lastDisplayedFreq > 1000 )
            {
                int freq = _freq / 1000;
                std::cout << "  - " << freq << "000Hz" << std::endl;
                _lastDisplayedFreq = _freq;
            }

            return blocksize;
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
    HFft<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

    // Display the final plot
    Config.IsFilterSpectrumPlot ? FFTMagnitudeShowPlot() : FFTMagnitudeShowGnuPlot();

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
        filter = HBiQuadFactory<HLowpassBiQuad<T>, T>::Create((HReader<T>*) &rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
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
    HFft<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

    // Display the final plot
    FFTMagnitudeShowGnuPlot();

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
        filter = HBiQuadFactory<HLowpassBiQuad<T>, T>::Create((HReader<T>*) rd, Config.FCutOff, Config.Rate, Config.Quality, Config.Gain, Config.Blocksize);
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
    FilterSpectrumReader<T> rd(2);

    // Create  filter
    HCombFilter<T> filter(&rd, Config.Rate, Config.Frequency, Config.Alpha, (Config.Alpha < 0 ? HCombFilter<T>::FEED_FORWARD : HCombFilter<T>::FEED_BACK), Config.Blocksize);

    // Create writer
    HCustomWriter<HFftResults> fftWriter(FFTMagnitudePlotWriter);

    // Create FFT
    HFft<T> fft(Config.Blocksize, 1, &fftWriter, new HHahnWindow<T>());

    // Buffer for the accumulated spectrum values
    aggregatedMagnitudeSpectrum = new double[Config.Blocksize / 2];

    // Create processor
    HStreamProcessor<T> proc(&fft, (HReader<T>*) &filter, Config.Blocksize, &terminated);
    std::cout << "Sweep from 0Hz - " << (Config.Rate / 2) << "Hz" << std::endl;
    proc.Run();

    // Display the final plot
    FFTMagnitudeShowGnuPlot();

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
    HCombFilter<T> filter(rd, Config.Rate, Config.Frequency, Config.Alpha, (Config.Alpha < 0 ? HCombFilter<T>::FEED_FORWARD : HCombFilter<T>::FEED_BACK), Config.Blocksize);

    // Create processor
    HStreamProcessor<T> proc(wr, (HReader<T>*) &filter, Config.Blocksize, &terminated);
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
    if( Config.Timer.duration() > 0 )
    {
        time_t stamp = Config.Timer.start();
        struct tm* timeInfo = localtime( &stamp );
        std::cout << "Operation starts at " << asctime(timeInfo) << std::endl;
        Config.Timer.wait();
        stamp = Config.Timer.stop();
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
    if( Config.IsFFTMagnitudePlot || Config.IsFFTMagnitudeGnuPlot )
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

    if( Config.IsFilterSpectrumPlot || Config.IsFilterSpectrumGnuPlot)
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
