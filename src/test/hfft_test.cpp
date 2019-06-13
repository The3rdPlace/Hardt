#include <stdio.h>
#include <iostream>

#include "test.h"

class HFft_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_fft_static_callback);
            UNITTEST(test_fft_static_callback_with_factory);
            UNITTEST(test_fft_member_callback);

            UNITTEST(test_fft);

            UNITTEST(test_fft_speed);
            UNITTEST(test_fft_speed_with_chunking);
        }

        const char* name()
        {
            return "HFft";
        }

    private:

        static int staticReady;
        static double* staticSpectrum;
        int ready;

        void writeToFFT(HFft<int16_t>* fft)
        {
            int16_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 9 ,8 ,7 ,6 ,5 ,4 ,3 ,2 ,1};
            ASSERT_IS_EQUAL(fft->Write(input, 20), 20);
        }

        static int staticCallback(double* data, size_t length)
        {
            staticReady++;
            //double r[64];

            if( staticSpectrum != NULL && data != NULL )
            {
                memcpy((void*) staticSpectrum, (void*) data, length * sizeof(double));
            }

            return length;
        }

        int memberCallback(double* data, size_t length)
        {
            ready++;
            return length;
        }

        class TestWindow : public HWindow<int16_t>
        {
            public:

                float ValueAt(int N, int n)
                {
                    return 1;
                }
        };

        void test_fft_static_callback()
        {
            staticReady = 0;
            auto fft = HFft<int16_t>(10, 1, new HCustomWriter<double>(staticCallback), new TestWindow);
            writeToFFT(&fft);
            ASSERT_IS_EQUAL(staticReady, 2);
        }

        void test_fft_static_callback_with_factory()
        {
            staticReady = 0;
            HFft<int16_t> fft(10, 1, HCustomWriter<double>::Create(staticCallback), new TestWindow);
            writeToFFT(&fft);
            ASSERT_IS_EQUAL(staticReady, 2);
        }

        void test_fft_member_callback()
        {
            ready = 0;
            HFft<int16_t> fft(10, 1, HCustomWriter<double>::Create<HFft_Test>(this, &HFft_Test::memberCallback), new TestWindow);
            writeToFFT(&fft);
            ASSERT_IS_EQUAL(ready, 2);
        }

        int test_fft_rate_frequency_N(int fs, int fg, int N)
        {
            // Get local buffer for the spectrum
            staticSpectrum = new double[N / 2];

            // Get a sinewave at 'fg' with samplerate 'fs' and amplitude '100'
            HSineGenerator<int16_t> g(fs, fg, 100);
            int16_t input[N];
            g.Read(input, N);

            // Run 'N' samples through the fft
            HFft<int16_t> fft(N, 1, new HCustomWriter<double>(staticCallback), new TestWindow);
            fft.Write(input, N);

            // Check if the spectrum contains the expected peak af 'fg'
            float fdelta = fs / (float) N;
            double max = 0;
            int avgFreq = 0;
            for( int i = 1; i < N / 2; i++ )
            {
                if( staticSpectrum[i] > max )
                {
                    max = staticSpectrum[i];
                    avgFreq = ((float) i * fdelta);
                }
            }

            // Clean up
            delete staticSpectrum;
            staticSpectrum = NULL;

            return avgFreq;
        }

        /* Test various samplerates and frequencies */
        float test_fft_freq(int freq)
        {
            int avg = 0;
            int num = 0;
            int fs;

            if( freq < 4000 )
            {
                fs = 8000;
                avg += test_fft_rate_frequency_N(fs, freq, 128);
                avg += test_fft_rate_frequency_N(fs, freq, 256);
                avg += test_fft_rate_frequency_N(fs, freq, 512);
                avg += test_fft_rate_frequency_N(fs, freq, 1024);
                avg += test_fft_rate_frequency_N(fs, freq, 2048);
                avg += test_fft_rate_frequency_N(fs, freq, 4096);
                num += 6;
            }

            if( freq < 11025 )
            {
                fs = 22050;
                avg += test_fft_rate_frequency_N(fs, freq, 128);
                avg += test_fft_rate_frequency_N(fs, freq, 256);
                avg += test_fft_rate_frequency_N(fs, freq, 512);
                avg += test_fft_rate_frequency_N(fs, freq, 1024);
                avg += test_fft_rate_frequency_N(fs, freq, 2048);
                avg += test_fft_rate_frequency_N(fs, freq, 4096);
                num += 6;
            }

            if( freq < 22050 )
            {
                fs = 44100;
                avg += test_fft_rate_frequency_N(fs, freq, 128);
                avg += test_fft_rate_frequency_N(fs, freq, 256);
                avg += test_fft_rate_frequency_N(fs, freq, 512);
                avg += test_fft_rate_frequency_N(fs, freq, 1024);
                avg += test_fft_rate_frequency_N(fs, freq, 2048);
                avg += test_fft_rate_frequency_N(fs, freq, 4096);
                num += 6;
            }

            fs = 48000;
            avg += test_fft_rate_frequency_N(fs, freq, 128);
            avg += test_fft_rate_frequency_N(fs, freq, 256);
            avg += test_fft_rate_frequency_N(fs, freq, 512);
            avg += test_fft_rate_frequency_N(fs, freq, 1024);
            avg += test_fft_rate_frequency_N(fs, freq, 2048);
            avg += test_fft_rate_frequency_N(fs, freq, 4096);
            num += 6;

            avg = avg / num;

            // Find out how much the measured frequency differs from
            // the expected, in percent
            int diff = std::abs(freq - avg);
            return (float) diff / (float) freq;
        }

        void test_fft()
        {
            float avg = 0;
            avg += test_fft_freq(440);
            avg += test_fft_freq(1000);
            avg += test_fft_freq(1750);
            avg += test_fft_freq(3200);
            avg += test_fft_freq(4500);
            avg += test_fft_freq(8200);
            avg += test_fft_freq(9300);
            avg += test_fft_freq(12020);
            avg += test_fft_freq(15027);
            avg += test_fft_freq(18000);
            avg += test_fft_freq(21000);
            avg += test_fft_freq(23400);

            // Get all differences, then take the average.
            // Require measurements to have a better tolerance
            // than 2 percent
            avg = (avg / 12) * 100;
            INFO("Average deviation of measured vs input frequency (percent): " << avg);
            ASSERT_IS_LESS_THAN(avg, (float) 0.30   );
        }

        int test_fft_speed_N(int N, int chunks, int baseline)
        {
            // Get local buffer for the spectrum
            staticSpectrum = new double[N / 2];

            // Get a 1KHz sinewave generator at samplerate 48K
            HSineGenerator<int16_t> g(48000, 1000, 100);
            int16_t input[N * chunks];
            g.Read(input, N * chunks);

            time_t start;
            time_t end;

            HFft<int16_t> fft(N, 1, new HCustomWriter<double>(staticCallback), new TestWindow);

            int ffts = 0;
            time(&start);
            do
            {
                fft.Write(input, N * chunks);
                ffts++;

                time(&end);
            }
            while( end - start < 3);
            int fftsPerSecond = (ffts / 3) * chunks;

            // Calculate how many blocks we need to process per second.
            // Require trice the amount to leave enough processing cycles
            // for overhead, presentation etc.
            int fftsNeeded = (48000 / N) * 3;
            INFO("FFT's per second needed at N = " << N << ": " << fftsNeeded);
            INFO("FFT's per second: " << fftsPerSecond << "  (" << baseline << ")");
            if( fftsPerSecond < fftsNeeded )
            {
                WARNING("This system is too slow to run realtime FFT's reliably");
            }
            return fftsPerSecond;
        }

        void test_fft_speed()
        {
            // Baseline values are measured when using a standard
            // implementation of the DFT on the development machine - they makes no
            // sense away from the author.
            int avg = 0;
            avg += test_fft_speed_N(512, 1, 1246);
            avg += test_fft_speed_N(2048, 1, 408);
            avg += test_fft_speed_N(8092, 1, 120);

            INFO("Average FFT's per second: " << avg / 3 << "  (784)");
        }

        void test_fft_speed_with_chunking()
        {
            // Baseline values are measured when using a standard
            // implementation of the DFT on the development machine - they makes no
            // sense away from the author.
            int avg = 0;
            avg += test_fft_speed_N(512, 5, 1770);
            avg += test_fft_speed_N(2048, 5, 380);
            avg += test_fft_speed_N(8092, 5, 110);

            INFO("Average FFT's per second with chunking: " << avg / 3 << "  (753)");
        }

} hfft_test;

int HFft_Test::staticReady = 0;

double* HFft_Test::staticSpectrum;