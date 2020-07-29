#include <stdio.h>
#include <iostream>

#include "test.h"

class HFft_Test: public Test
{
public:

    void run()
    {
        UNITTEST(test_fft_with_window);
        UNITTEST(test_fft_without_window);
        UNITTEST(test_spectrum);
        UNITTEST(test_ifft);
        UNITTEST(test_hilbert);
    }

    const char* name()
    {
        return "HFft";
    }

private:

    void test_fft_with_window()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t input[1024];
        osc.Read(input, 1024);

        HHahnWindow<int16_t> window;
        HFft<int16_t> fft(1024, &window);

        std::complex<double> output[1024];
        fft.FFT(input, output);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 127 ) {
                ASSERT_IS_BETWEEN(std::abs(output[i]), (double) 12810, (double) 12811);
            } else if( i == 128 ) {
                ASSERT_IS_BETWEEN(std::abs(output[i]), (double) 25343, (double) 25344);
            } else if( i == 129 ) {
                ASSERT_IS_BETWEEN(std::abs(output[i]), (double) 12812, (double) 12813);
            } else {
                ASSERT_IS_LESS_THAN(std::abs(output[i]), (double) 35);
            }
        }
    }

    void test_fft_without_window()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t input[1024];
        osc.Read(input, 1024);

        HFft<int16_t> fft(1024);

        std::complex<double> output[1024];
        fft.FFT(input, output);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 128 ) {
                ASSERT_IS_BETWEEN(std::abs(output[i]), (double) 51304, (double) 51305);
            } else if( i == 384 ) {
                // This is a small spike due to the generator not being totally correct
                ASSERT_IS_BETWEEN(std::abs(output[i]), (double) 104, (double) 105);
            }else {
                ASSERT_IS_EQUAL(std::abs(output[i]), (double) 0);
            }
        }
    }

    void test_spectrum()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t input[1024];
        osc.Read(input, 1024);

        HHahnWindow<int16_t> window;
        HFft<int16_t> fft(1024, &window);

        double spectrum[512];
        fft.FFT(input, spectrum);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 127 ) {
                ASSERT_IS_BETWEEN(spectrum[i], (double) 12810, (double) 12811);
            } else if( i == 128 ) {
                ASSERT_IS_BETWEEN(spectrum[i], (double) 25343, (double) 25344);
            } else if( i == 129 ) {
                ASSERT_IS_BETWEEN(spectrum[i], (double) 12812, (double) 12813);
            } else {
                ASSERT_IS_LESS_THAN(spectrum[i], (double) 35);
            }
        }
    }

    void test_ifft()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t input[1024];
        osc.Read(input, 1024);

        HHahnWindow<int16_t> window;
        HFft<int16_t> fft(1024, &window);

        std::complex<double> transformed[1024];
        fft.FFT(input, transformed);

        int16_t output[1024];
        fft.IFFT(transformed, output);

        // Reapply fft to compare spectrum of the inverse-fft'ed signal
        fft.FFT(input, transformed);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 127 ) {
                ASSERT_IS_BETWEEN(std::abs(transformed[i]), (double) 12810, (double) 12811);
            } else if( i == 128 ) {
                ASSERT_IS_BETWEEN(std::abs(transformed[i]), (double) 25343, (double) 25344);
            } else if( i == 129 ) {
                ASSERT_IS_BETWEEN(std::abs(transformed[i]), (double) 12812, (double) 12813);
            } else {
                ASSERT_IS_LESS_THAN(std::abs(transformed[i]), (double) 35);
            }
        }
    }

    void test_hilbert()
    {
        HSineGenerator<int16_t> osc(H_SAMPLE_RATE_8K, 1000, 100);
        int16_t input[1024];
        osc.Read(input, 1024);

        HFft<int16_t> fft(1024);

        int16_t output[1024];
        fft.HILBERT(input, output);

        // Apply fft to compare spectrum of the inverse-fft'ed signal
        double spectrum[1024];
        double phase[512];
        fft.FFT(output, spectrum, phase);

        // The small amount of data and integer rounding will move the center slightly
        // away from the mathematically correct bin numbers
        for(int i = 0; i < 512; i++ ) {
            if( i == 128 ) {
                ASSERT_IS_BETWEEN(spectrum[i], (double) 25471, (double) 25472);
                ASSERT_IS_BETWEEN(phase[i], (double) 1.5707, (double) 1.5708);
            } else if( i == 384 ) {
                // This is a small spike due to the generator not being totally correct
                ASSERT_IS_BETWEEN(spectrum[i], (double) 128, (double) 129);
            }else {
                ASSERT_IS_EQUAL(spectrum[i], (double) 0);
            }
        }
    }

} hfft_test;