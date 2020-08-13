#ifndef __HFFTOUTPUT_H
#define __HFFTOUTPUT_H

#include <complex>

#include "hfft.h"
#include "hmultiplier.h"
#include "hfirfilter.h"
#include "hdecimator.h"
#include "hmemorywriter.h"

/**
    Holds the result of a number of dfft's
*/
struct HFftResults
{
    /** The calculated/measured spectrum */
    double* Spectrum;

    /** The complex spectrum, in case you need to do phase calculations */
    std::complex<double>* Result;

    /** Size of spectrum (bins) */
    int Size;
};

/**
    Calculate a number of dfft's, if applicable then average the spectrum over
    the number of dfft's, then write the calculated spectrum results to the
    given writer (usually a HCustomWRiter)
*/
template <class T>
class HFftOutput : public HOutput<T, HFftResults>
{
    private:

        int _size;
        int _average;

        double* _spectrum;
        std::complex<double>* _fftResult;
        std::complex<double>* _result;

        int _count;
        HWindow<T>* _window;

        T* _buffer;

        HFft<T>* _fft;

        void Init();

        int _zoomFactor;
        int _zoomCenter;
        int _zoomPoints;
        H_SAMPLE_RATE _zoomRate;
        T* _zoomOutput;
        HInputWriter<T>* _zoomInputWriter;
        HMultiplier<T>* _zoomMultiplier;
        HFirFilter<T>* _zoomFilter;
        HDecimator<T>* _zoomDecimator;
        HMemoryWriter<T>* _zoomMemoryWriter;
        void SetZoom();
        T* Zoom(T* src, size_t size);

    public:

        /** Create a new HFft output that writes to a writer

            Parameters:

              size = The FFT size (1024 is a good choice for analytic use, 256 is recommended for realtime applications)

              average = The number of FFT's to accumulate and average before calling the result writer. Note that if you
                        use anything but 1, you can not run your signal through the output multiple times since that
                        would mix results in the averaging phase (say if you where zooming in)

              writer = The result writer (most likely a HCustomerWriter<HFftResults>*)

              window = The window to use before calculating the FFT

              zoomFactor = Zoom factor. If larger than 1, then the spectrum is zoomed around 'center' (in relation to 'size').
                           The factor must divide up into 'size' with an integer result (1024/4 = ok, 1024/3 = not-ok!).

              zoomCenter = The center bin (frequency), must be in the [0-size/2] range. Use the static HFftOutput::BinAtFrequency()
                           method if you do not already know the bin number.

              zoomPoints = The number of FIR coefficients used to filter the input before resampling. Use something like 15 if
                           you need speed instead of precision, something in the range 51-101 for moderate speed and acceptable
                           precision and go high if you need precision and no degradation of signals in the zoomed range (257 or higher)

              zoomRate = The samplerate of the input samples

              If a zoomFactor of 1 (default value) is given, the parameters zoomCenter and zoomPoints is not used at all.
        */
        HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window, int zoomFactor = 1, int zoomCenter = 0, int zoomPoints = 0, H_SAMPLE_RATE zoomRate = 0);

        /** Create a new HFft output that registers with an upstream writer

            Parameters:

              size = The FFT size (1024 is a good choice for analytic use, 256 is recommended for realtime applications)

              average = The number of FFT's to accumulate and average before calling the result writer. Note that if you
                        use anything but 1, you can not run your signal through the output multiple times since that
                        would mix results in the averaging phase (say if you where zooming in)

              consumer = The upstream writer that this HFftOutput registers with. Make sure that you create a HWriter<HFftResults>
                         that registers with this HFftOutput before pushing samples through.  (in most cases you would use
                         a HCustomWriter<HFftResults> object)

              window = The window to use before calculating the FFT

              zoomFactor = Zoom factor. If larger than 1, then the spectrum is zoomed around 'center' (in relation to 'size').
                           The factor must divide up into 'size' with an integer result (1024/4 = ok, 1024/3 = not-ok!).

              zoomCenter = The center bin (frequency), must be in the [0-size/2] range. Use the static HFftOutput::BinAtFrequency()
                           method if you do not already know the bin number.

              zoomPoints = The number of FIR coefficients used to filter the input before resampling. Use something like 15 if
                           you need speed instead of precision, something in the range 51-101 for moderate speed and acceptable
                           precision and go high if you need precision and no degradation of signals in the zoomed range (257 or higher)

              zoomRate = The samplerate of the input samples

              If a zoomFactor of 1 (default value) is given, the parameters zoomCenter and zoomPoints is not used at all.
        */
        HFftOutput(int size, int average, HWriterConsumer<T>* consumer, HWindow<T>* window, int zoomFactor = 1, int zoomCenter = 0, int zoomPoints = 0, H_SAMPLE_RATE zoomRate = 0);

        /** Default destructor */
        ~HFftOutput()
        {
            delete _spectrum;
            delete _fftResult;
            delete _buffer;
            delete _fft;

            delete _zoomInputWriter;
            delete _zoomMultiplier;
            delete _zoomFilter;
            delete _zoomDecimator;
            delete _zoomOutput;
        }

        /** Process a block of samples */
        int Output(T* src, size_t size);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No ruther propagation of commands
            return true;
        }

        /** Given the sampling rate, return the right frequency of the first bin */
        int StartFrequency(H_SAMPLE_RATE rate) {
            if( _zoomFactor == 1 ) {
                return FrequenciesPerBin(rate);
            }
            return (float) CenterFrequency(rate) - (((float) _size / 4) * FrequenciesPerBin(rate)) + FrequenciesPerBin(rate);
        }

        /** Given the sampling rate, return the right frequency of the last bin */
        int StopFrequency(H_SAMPLE_RATE rate) {
            if( _zoomFactor == 1 ) {
                return FrequenciesPerBin(rate) * ((float) _size / 2);
            }
            return (float) CenterFrequency(rate) + (((float) _size / 4) * FrequenciesPerBin(rate));
        }

        /** Given the sampling rate, return the center frequency - if zoom is used */
        int CenterFrequency(H_SAMPLE_RATE rate) {

            if( _zoomFactor == 1 ) {
                return rate / 4;
            }

            return _zoomCenter == 0
                           ? rate / 4
                           : _zoomCenter;
        }

        /** Given the sampling rate, return the number of frequencies per bin */
        float FrequenciesPerBin(H_SAMPLE_RATE rate) {
            return (((float) rate / 2) / ((float) _size / 2)) / (float) _zoomFactor;
        }

        /** Given the input sample rate and having a zoom factor, return the
            output sample rate after resampling */
        int Rate(H_SAMPLE_RATE rate) {
            if( _zoomFactor == 1 ) {
                return rate;
            }
            return rate / _zoomFactor;
        }
};

#endif
