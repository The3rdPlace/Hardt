#ifndef __HFFT_H
#define __HFFT_H

#include <complex>
#include <valarray>
#include <complex.h>

#include "hwindow.h"

/** (D)FFT related functions */
template <class T>
class HFft {

    private:

        HWindow<T>* _window;
        int _size;

        T* _fftBuffer;
        std::complex<double>* _hilbertBuffer;

        void FFT(std::valarray<std::complex<double>>& x)
        {
            const size_t N = x.size();
            if (N <= 1) return;

            // divide
            std::valarray<std::complex<double>> even = x[std::slice(0, N/2, 2)];
            std::valarray<std::complex<double>>  odd = x[std::slice(1, N/2, 2)];

            // conquer
            FFT(even);
            FFT(odd);

            // combine
            for (size_t k = 0; k < N/2; ++k)
            {
                std::complex<double> t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
                x[k    ] = even[k] + t;
                x[k+N/2] = even[k] - t;
            }
        }

    public:

        /** Construct a new FFT class 
            
            Parameters:
                size = fft size
                window = a window to smoothen the first and last parts of the input samples to avoid
                         high frequency "splatter"
        */
        HFft(int size, HWindow<T>* window = nullptr):
            _window(window),
            _size(size) {

            // Set window size
            if( _window != nullptr ) {
                _window->SetSize(_size);
            }

            // Allocate a buffer for intermediate results in the fft functions
            _fftBuffer = new T[_size];

            // Allocate a buffer for intermediate results in the hilbert function
            _hilbertBuffer = new std::complex<double>[_size];
        }

        ~HFft() {
            delete _fftBuffer;
            delete _hilbertBuffer;
        }

        /** Calculate the fft for complex samples and place the complex results into a buffer

        Parameters:
            src = Input buffer, must contain 'size' number of complex samples
            result = The output result array, must have enough room for 'size' values
        */
        void FFT(std::complex<T>* src, std::complex<double>* result) {

            // Prepare a set of complex values
            std::valarray<std::complex<double>> x(_size);
            for( int i = 0; i < _size ; i++ )
            {
                x[i] = std::complex<double>(src[i].real(), src[i].imag());
            }

            // Calculate the FFT
            FFT(x);

            // Convert to simple array with bins for positive AND negative frequencies
            for( int i = 0; i < _size; i++ )
            {
                result[i] = x[i];
            }
        }

        /** Calculate the fft for samples and place the complex results into a buffer
            
            Parameters:
                src = Input buffer, must contain 'size' number of samples
                result = The output result array, must have enough room for 'size' values
        */
        void FFT(T* src, std::complex<double>* result, bool window = true) {

            // Run the input buffer through a window function, if any is given
            if( window && _window != nullptr ) {
                _window->Apply(src, _fftBuffer, _size);
            } else {
                memcpy((void*) _fftBuffer, (void*) src, _size * sizeof(T));
            }

            // Prepare a set of complex values
            std::valarray<std::complex<double>> x(_size);
            for( int i = 0; i < _size ; i++ )
            {
                x[i] = std::complex<double>(_fftBuffer[i], 0);
            }

            // Calculate the FFT
            FFT(x);

            // Convert to simple array with bins for positive AND negative frequencies
            for( int i = 0; i < _size; i++ )
            {
                result[i] = x[i];
            }
        }

        /** Calculate the fft for samples and place the real values spectrum into a buffer
            
            Parameters:
                src = Input buffer, must contain 'size/2' number of samples
                spectrum = The output spectrum array, must have enough room for 'size'/2 values
        */
        void FFT(T* src, double* spectrum, double* phase = nullptr) {

            // Allocate a buffer for the complex spectrum results
            std::complex<double>* result = new std::complex<double>[_size];

            // Run fft on the input
            FFT(src, result);

            // convert to real valued spectrum powers for each bin
            for( int i = 0; i < _size / 2; i++ )
            {
                // Get absolute value at point n
                spectrum[i] = std::abs(result[i]);

                if( phase != nullptr )
                {
                    phase[i] = std::arg(result[i]);
                }
            }

            // Cleanup
            delete[] result;
        }

        /** Calculate the ifft for complex values and place the samples into a buffer

            Parameters:
                src = Input buffer, must contain 'size/2' number of complex values representing positive frequencies
                result = The output samples array, must have enough room for 'size' samples
        */
        void IFFT(std::complex<double>* src, T* result) {

            // Prepare the input array to the fft function, filled with the complex conjugate of the input values
            std::valarray<std::complex<double>> x(_size);
            for( int i = 0; i < _size ; i++ )
            {
                x[i] = src[i];
            }
            x.apply(std::conj);

            // Calculate the FFT
            FFT(x);

            // Convert to simple array with the realvalued signal samples
            double factor = 1.0 / _size;
            x.apply(std::conj);
            for( int i = 0; i < _size; i++ )
            {
                result[i] = x[i].real() * factor;
            }
        }

        /** Run the input signal through a hilbert transform (delay 90 degrees) */
        void HILBERT(T* src, T* dest) {
            FFT(src, _hilbertBuffer, false);

            for(int i = _size / 2; i < _size; i++ ) {
                _hilbertBuffer[i] = 0;
            }

            IFFT(_hilbertBuffer, dest);
        }

        /** Return the spectrum for a frequency response. For use when
            one needs the spectrum of a filter, for example to do
            frequency domain filtering.

            This would typically be a set of FIR coefficients, but
            can be any frequency response as long as it is not infinite
            and has a length <= fft-size/2.

            'spectrum' must have enough room for 'fft-size' samples.
        */
        void SPECTRUM(T* response, int length, std::complex<double>* spectrum) {

            // Sanity check
            if( length > _size / 2 ) {
                throw new HInitializationException("Length must not exceed fft-size/2 in call to HFft::SPECTRUM");
            }

            // Initialize buffer
            T* buffer = new T[_size];
            memset((void*) buffer, 0, sizeof(T) * _size);

            // Copy frequency response into buffer
            memcpy((void*) buffer, (void*) response, sizeof(T) * length);

            // Take FFT of the frequency response
            FFT(buffer, spectrum);
        }
};

#endif
