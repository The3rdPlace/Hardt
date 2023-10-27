#ifndef __HFFTOUTPUT_H
#define __HFFTOUTPUT_H

#include <complex>

#include "hfft.h"
#include "hmultiplier.h"
#include "hfirfilter.h"
#include "hdecimator.h"
#include "hmemorywriter.h"
#include "houtput.h"
#include "hbaseband.h"

/**
    Holds the result of a number of dfft's
*/
struct HFftResults
{
    /**
     * The calculated/measured spectrum
     */
    double* Spectrum;

    /**
     * The complex spectrum, in case you need to do phase calculations
     */
    std::complex<double>* Result;

    /**
     * Size of spectrum (bins)
     */
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
        bool _isIq;
        int _skip;
        int _skipped;

        double* _spectrum;
        std::complex<double>* _fftResult;
        std::complex<double>* _result;

        int _count;
        HWindow<T>* _window;

        T* _buffer;
        std::complex<T>* _cxInput;

        HFft<T>* _fft;

        void Init(std::string id);

        int _zoomFactor;
        int _zoomCenter;
        H_SAMPLE_RATE _zoomRate;
        T* _zoomOutput;
        HInputWriter<T>* _zoomInputWriter;
        HBaseband<T>* _zoomBaseband;
        HDecimator<T>* _zoomDecimator;
        HMemoryWriter<T>* _zoomMemoryWriter;
        T* Zoom(T* src, size_t size);
        bool _zoomEnabled;

    public:

        /** Create a new HFft output that writes to a writer
         *
         * @param id Element identifier
         * @param size The FFT size (1024 is a good choice for analytic use,
         *        256 is recommended for realtime applications)
         * @param average The number of FFT's to accumulate and average before
         *                calling the result writer. Note that if you use anything
         *                but 1, you can not run your signal through the output
         *                multiple times since that would mix results in the averaging
         *                phase (say if you where zooming in)
         * @param skip Take each 'skip' sample and skip the rest
         * @param writer The result writer (most likely a HCustomerWriter<HFftResults>*)
         * @param window The window to use before calculating the FFT
         * @param zoomRate The samplerate of the input samples
         * @param zoomFactor Zoom factor. If larger than 1, then the spectrum is zoomed
         *                   around 'center' (in relation to 'size'). The factor must
         *                   divide up into 'size' with an integer result (1024/4 = ok,
         *                   1024/3 = not-ok!).
         * @param zoomCenter The center bin (frequency), must be in the [0-size/2] range.
         *                   Use the static HFftOutput::BinAtFrequency() method if you do
         *                   not already know the bin number.
         *
         * If you intend to use scaling, you _must_ initialize the HFftOutput with a
         * samplerate. You can set the factor to 1 initially to disable zooming. But if you
         * have not provided zoomRate != 0, zooming will not be setup and be available.!
         */
        HFftOutput(std::string id, int size, int average, int skip, HWriter<HFftResults>* writer, HWindow<T>* window, H_SAMPLE_RATE zoomRate = 0, int zoomFactor = 1, int zoomCenter = 0);

        /**
         * Create a new HFft output that registers with an upstream writer
         *
         * @param id Element identifier
         * @param size The FFT size (1024 is a good choice for analytic use,
         *        256 is recommended for realtime applications)
         * @param average The number of FFT's to accumulate and average before
         *                calling the result writer. Note that if you use anything
         *                but 1, you can not run your signal through the output
         *                multiple times since that would mix results in the averaging
         *                phase (say if you where zooming in)
         * @param skip Take each 'skip' sample and skip the rest
         * @param consumer The upstream writer that should write to this component
         * @param window The window to use before calculating the FFT
         * @param zoomRate The samplerate of the input samples
         * @param zoomFactor Zoom factor. If larger than 1, then the spectrum is zoomed
         *                   around 'center' (in relation to 'size'). The factor must
         *                   divide up into 'size' with an integer result (1024/4 = ok,
         *                   1024/3 = not-ok!).
         * @param zoomCenter The center bin (frequency), must be in the [0-size/2] range.
         *                   Use the static HFftOutput::BinAtFrequency() method if you do
         *                   not already know the bin number.
         *
         * If you intend to use scaling, you _must_ initialize the HFftOutput with a
         * samplerate. You can set the factor to 1 initially to disable zooming. But if you
         * have not provided zoomRate != 0, zooming will not be setup and be available.!
         */
        HFftOutput(std::string id, int size, int average, int skip, HWriterConsumer<T>* consumer, HWindow<T>* window, H_SAMPLE_RATE zoomRate = 0, int zoomFactor = 1, int zoomCenter = 0);

        /** Create a new HFft output that writes to a writer
         *
         * @param id Element identifier
         * @param size The FFT size (1024 is a good choice for analytic use,
         *        256 is recommended for realtime applications)
         * @param average The number of FFT's to accumulate and average before
         *                calling the result writer.
         * @param skip Take each 'skip' sample and skip the rest
         * @param writer The result writer (most likely a HCustomerWriter<HFftResults>*)
         * @param window The window to use before calculating the FFT
         * @param isIq If set to true then the input must be IQ data and the returned
         *             spectrum then contains negative as well as positive frequencies.
         * If you intend to use scaling, you _must_ initialize the HFftOutput with a
         * samplerate. You can set the factor to 1 initially to disable zooming. But if you
         * have not provided zoomRate != 0, zooming will not be setup and be available.!
         */
        HFftOutput(std::string id, int size, int average, int skip, HWriter<HFftResults>* writer, HWindow<T>* window, bool isIq);

        /** Create a new HFft output that writes to a writer
         *
         * @param id Element identifier
         * @param size The FFT size (1024 is a good choice for analytic use,
         *        256 is recommended for realtime applications)
         * @param average The number of FFT's to accumulate and average before
         *                calling the result writer.
         * @param skip Take each 'skip' sample and skip the rest
         * @param consumer The upstream writer that should write to this component
         * @param window The window to use before calculating the FFT
         * @param isIq If set to true then the input must be IQ data and the returned
         *             spectrum then contains negative as well as positive frequencies.
         * If you intend to use scaling, you _must_ initialize the HFftOutput with a
         * samplerate. You can set the factor to 1 initially to disable zooming. But if you
         * have not provided zoomRate != 0, zooming will not be setup and be available.!
         */
        HFftOutput(std::string id, int size, int average, int skip, HWriterConsumer<T>* consumer, HWindow<T>* window, bool isIq);

        /**
         * Default destructor
         */
        ~HFftOutput()
        {
            delete _spectrum;
            delete _fftResult;
            delete _buffer;
            delete _cxInput;
            delete _fft;

            if( _zoomEnabled ) {
                delete _zoomInputWriter;
                delete _zoomBaseband;
                delete _zoomDecimator;
                delete _zoomOutput;
            }
        }

        /**
         * Process a block of samples
         */
        int Output(T* src, size_t size);

        /**
         * Execute or carry through a command
         */
        bool Command(HCommand* command) {
            // No Further propagation of commands
            return true;
        }

        /**
         * Get the width of the FFT spectrum (how many frequencies is covered)
         */
        int Width() {
            if( !_zoomEnabled ) {
                HError("HFftOutput with zoom disabled cannot report FFT frequency span");
                return 0;
            }

            return (_zoomRate / 2) / _zoomFactor;
        }

        /**
         * Get the leftmost frequency of the selected fft window
         */
        int Left() {
            if( !_zoomEnabled ) {
                HError("HFftOutput with zoom disabled cannot report left FFT frequency");
                return 0;
            }

            return Center() - (Width() / 2);
        }

        /**
         * Get the rightmost frequency of the selected fft window
         */
        int Right() {
            if( !_zoomEnabled ) {
                HError("HFftOutput with zoom disabled cannot report right FFT frequency");
                return 0;
            }

            return Center() + (Width() / 2);
        }

        /**
         * Get the center FFT frequency
         */
        int Center() {
            if( !_zoomEnabled ) {
                HError("HFftOutput with zoom disabled cannot report center FFT frequency");
                return 0;
            }

            return _zoomCenter == 0
                           ? _zoomRate / 4
                           : _zoomCenter;
        }

        /**
         * Return the number of frequencies per bin
         */
        float BinSize() {
            if( !_zoomEnabled ) {
                HError("HFftOutput with zoom disabled cannot report FFT bin size");
                return 0;
            }

            return (((float) _zoomRate / 2) / ((float) _size / 2)) / (float) _zoomFactor;
        }

        /**
         * Given a samplerate, return the number of frequencies per bin
         */
        float BinSize(H_SAMPLE_RATE rate) {
            return ((float) rate / 2) / ((float) _size / 2);
        }

        /**
         * Return the output sample rate after resampling
         */
        H_SAMPLE_RATE Rate() {
            if( !_zoomEnabled ) {
                HError("HFftOutput with zoom disabled cannot report FFT rate");
                return 0;
            }

            return _zoomRate / _zoomFactor;
        }

        /**
         * Change zoom factor and center
         */
        void SetZoom(int zoomFactor, int zoomCenter);

        /** Initialize before first read/write */
        bool Start() {

            // Report first (empty) result
            HLog("Initialize spectrum");
            memset((void*) _spectrum, 0, (_size / 2) * sizeof(double));
            memset((void*) _result, 0, (_size / 2) * sizeof(std::complex<double>));
            HFftResults results;
            results.Spectrum = &_spectrum[0];
            results.Result = &_result[0];
            results.Size = _size / 2;
            HOutput<T, HFftResults>::Ready(&results, 1);

            return true;
        }

        /** Cleanup after last read/write */
        bool Stop() {

            // Report one last (empty) result
            HLog("Clear spectrum");
            memset((void*) _spectrum, 0, (_size / 2) * sizeof(double));
            memset((void*) _result, 0, (_size / 2) * sizeof(std::complex<double>));
            HFftResults results;
            results.Spectrum = &_spectrum[0];
            results.Result = &_result[0];
            results.Size = _size / 2;
            HOutput<T, HFftResults>::Ready(&results, 1);

            return true;
        }
};

#endif
