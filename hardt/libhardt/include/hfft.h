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

        T* _buffer;

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
        HFft(int size, HWindow<T>* window):
            _window(window),
            _size(size) {

            // Set window size
            _window->SetSize(_size);

            // Allocate a buffer for intermediate results
            _buffer = new T[_size];
        }

        ~HFft() {
            delete _buffer;
        }

        /** Calculate the fft for samples and place the complex results into a buffer 
            
            Parameters:
                src = Input buffer, must contain 'size' number of samples
                result = The output result array, must have enough room for 'size' values
        */
        void FFT(T* src, std::complex<double>* result) {

            // Run the input buffer through a window function, if any is given
            _window->Apply(src, _buffer, _size);

            // Prepare a set of complex values
            std::valarray<std::complex<double>> x(_size);
            for( int i = 0; i < _size ; i++ )
            {
                x[i] = std::complex<double>(_buffer[i], 0);
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
        void FFT(T* src, double* spectrum) {

            // Allocate a buffer for the complex spectrum results
            // Do not forget to zero the result buffer as new results are being added to the 
            // current values, not overriding them!
            std::complex<double>* result = new std::complex<double>[_size];
            memset((void*) result, 0, _size * sizeof(std::complex<double>));

            // Run fft on the input
            FFT(src, result);

            // convert to real valued spectrum powers for each bin
            for( int i = 0; i < _size / 2; i++ )
            {
                // Get absolute value at point n
                spectrum[i] = std::abs(result[i]);
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
        for( int i = 0; i < _size; i++ )
        {
            x[i] = conj(src[i]);
        }

        // Calculate the FFT
        FFT(x);

        // Convert to simple array with the realvalued signal samples
        double factor = 1.0 / _size;
        for( int i = 0; i < _size; i++ )
        {
            result[i] = conj(x[i]).real() * factor;
        }
    }

};

#endif
