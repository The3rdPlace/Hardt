#ifndef __HFFTOUTPUT_H
#define __HFFTOUTPUT_H

#include <complex>

#include "hfft.h"

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

        int _zoom;
        int _center;

        double* _spectrum;
        std::complex<double>* _fftResult;
        std::complex<double>* _result;

        int _count;
        HWindow<T>* _window;

        T* _buffer;

        HFft<T>* _fft;

        void Init();

        T* Zoom(T* src, size_t size);

    public:

        /** Create a new HFft output that writes to a writer

            Parameters:
              size = The FFT size (1024 is a good choice for analytic use, 256 is recommended for realtime applications)
              average = The number of FFT's to accumulate and average before calling the result writer
              writer = The result writer (most likely a HCustomerWriter<HFftResults>*)
              window = The window to use before calculating the FFT
              zoom = Zoom factor. If larger than 1, then the spectrum is zoomed around 'center' (in relation to 'size').
                     The factor must divide up into 'size' with an integer result (1024/3 = ok, 1024/3 = not-ok!).
              center = The center bin (frequency) when zooming. If 'zoom'=1 then 'center' is ignored. The value 0 (zero)
                       means the midpoint over the lower (size/zoom) bins. (zoom and keep the lowest part of the spectrum.
         * */
        HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window, int zoom = 1, int center = 0);

        /** Create a new HFft output that registers with an upstream writer

            Parameters:
              size = The FFT size (1024 is a good choice for analytic use, 256 is recommended for realtime applications)
              average = The number of FFT's to accumulate and average before calling the result writer
              writer = The result writer (most likely a HCustomerWriter<HFftResults>*)
              window = The window to use before calculating the FFT
              zoom = Zoom factor. If larger than 1, then the spectrum is zoomed around 'center' (in relation to 'size').
                     The factor must divide up into 'size' with an integer result (1024/3 = ok, 1024/3 = not-ok!).
              center = The center bin (frequency) when zooming. If 'zoom'=1 then 'center' is ignored. The value 0 (zero)
                       means the midpoint over the input spectrum.
         */
        HFftOutput(int size, int average, HWriterConsumer<T>* consumer, HWindow<T>* window, int zoom = 1, int center = 0);

        /** Default destructor */
        ~HFftOutput()
        {
            delete _spectrum;
            delete _fftResult;
            delete _buffer;
            delete _fft;
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
            if( _zoom == 1 ) {
                return FrequenciesPerBin(rate);
            }
            return (float) CenterFrequency(rate) - (((float) _size / 4) * FrequenciesPerBin(rate)) + FrequenciesPerBin(rate);
        }

        /** Given the sampling rate, return the right frequency of the last bin */
        int StopFrequency(H_SAMPLE_RATE rate) {
            if( _zoom == 1 ) {
                return FrequenciesPerBin(rate) * ((float) _size / 2);
            }
            return (float) CenterFrequency(rate) + (((float) _size / 4) * FrequenciesPerBin(rate));
        }

        /** Given the sampling rate, return the center frequency - if zoom is used */
        int CenterFrequency(H_SAMPLE_RATE rate) {

            if( _zoom == 1 ) {
                return rate / 4;
            }

            return _center == 0
                           ? rate / 4
                           : _center;
        }

        /** Given the sampling rate, return the number of frequencies per bin */
        float FrequenciesPerBin(H_SAMPLE_RATE rate) {
            return (((float) rate / 2) / ((float) _size / 2)) / (float) _zoom;  // rate/size
        }

        /** Given the input sample rate and having a zoom factor, return the
            output sample rate after resampling */
        int Rate(H_SAMPLE_RATE rate) {
            if( _zoom == 1 ) {
                return rate;
            }
            return rate / _zoom;
        }
};

#endif
