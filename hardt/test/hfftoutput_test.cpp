#include <stdio.h>
#include <iostream>

#include "test.h"

class HFftOutput_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_fft_static_callback);
            UNITTEST(test_fft_static_callback_with_factory);
            UNITTEST(test_fft_member_callback);

            UNITTEST(test_fft);

            UNITTEST(test_zoom_helpers);

            INTEGRATIONTEST(test_fft_speed);
            INTEGRATIONTEST(test_fft_speed_with_chunking);
        }

        const char* name()
        {
            return "HFftOutput";
        }

    private:

        static int staticReady;
        static double* staticSpectrum;
        int ready;

        void writeToFFT(HFftOutput<int16_t>* fft)
        {
            int16_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 9 ,8 ,7 ,6 ,5 ,4 ,3 ,2 ,1};
            ASSERT_IS_EQUAL(fft->Write(input, 20), 20);
        }

        static int staticCallback(HFftResults* data, size_t length)
        {
            staticReady++;

            if( staticSpectrum != NULL && data != NULL )
            {
                memcpy((void*) staticSpectrum, (void*) data[0].Spectrum, data[0].Size * sizeof(double));
            }

            return length;
        }

        int memberCallback(HFftResults* data, size_t length)
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
            auto fft = HFftOutput<int16_t>(10, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow);
            writeToFFT(&fft);
            ASSERT_IS_EQUAL(staticReady, 2);
        }

        void test_fft_static_callback_with_factory()
        {
            staticReady = 0;
            HFftOutput<int16_t> fft(10, 1, HCustomWriter<HFftResults>::Create(staticCallback), new TestWindow);
            writeToFFT(&fft);
            ASSERT_IS_EQUAL(staticReady, 2);
        }

        void test_fft_member_callback()
        {
            ready = 0;
            HFftOutput<int16_t> fft(10, 1, HCustomWriter<HFftResults>::Create<HFftOutput_Test>(this, &HFftOutput_Test::memberCallback), new TestWindow);
            writeToFFT(&fft);
            ASSERT_IS_EQUAL(ready, 2);
        }

        int test_fft_rate_frequency_N(int fs, int fg, int N, int factor = 1, int center = 0, int points = 0, int iterations = 1)
        {
            // Get local buffer for the spectrum
            staticSpectrum = new double[N / 2];

            // Create the fft
            HFftOutput<int16_t> fft(N, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow, points,
                                    fs, factor, center);

            // Get a sinewave at 'fg' with samplerate 'fs' and amplitude '100'
            HSineGenerator<int16_t> g(fs, fg, 100);
            int16_t input[N];

            // Run a number of read-write iterations
            for( int it = 0; it < iterations; it++ ) {

                // Run 'N' samples through the fft
                g.Read(input, N);
                fft.Write(input, N);
            }

            // Check if the spectrum contains the expected peak af 'fg'
            int avgFreq = 0;
            float fdelta = fs / (float) N;
            double max = 0;
            for (int i = 1; i < N / 2; i++) {
                if (staticSpectrum[i] > max) {
                    max = staticSpectrum[i];
                    avgFreq = ((float) i * fdelta);

                }
            }

            // Clean up
            delete staticSpectrum;
            staticSpectrum = NULL;

            // Return the frequency with maximum spectral output
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
            ASSERT_IS_LESS_THAN(avg, (float) 0.60   );
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

            HFftOutput<int16_t> fft(N, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow);

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

        void test_zoom_helpers()
        {
            // Zoom = 1 (no zoom)
            HFftOutput<int16_t> fft_1(1024, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow);
            ASSERT_IS_EQUAL(fft_1.Width(), 0);
            ASSERT_IS_EQUAL(fft_1.Center(), 0);
            ASSERT_IS_EQUAL(fft_1.Left(), 0);
            ASSERT_IS_EQUAL(fft_1.Right(), 0);
            ASSERT_IS_EQUAL((int) (fft_1.BinSize(H_SAMPLE_RATE_8K) * 10000), 78125);
            ASSERT_IS_EQUAL( test_fft_rate_frequency_N(H_SAMPLE_RATE_8K, 1000, 1024), 1000 );

            // Zoom = 2, center = 0 (middle)
            HFftOutput<int16_t> fft_2(1024, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow, 15, H_SAMPLE_RATE_8K, 2, 0);
            ASSERT_IS_EQUAL(fft_2.Rate(), 4000);
            ASSERT_IS_EQUAL(fft_2.Width(), 2000);
            ASSERT_IS_EQUAL(fft_2.Center(), 2000);
            ASSERT_IS_EQUAL(fft_2.Left(), 1000);
            ASSERT_IS_EQUAL(fft_2.Right(), 3000);
            ASSERT_IS_EQUAL((int) (fft_2.BinSize() * 100000), 390625);
            ASSERT_IS_EQUAL( test_fft_rate_frequency_N(H_SAMPLE_RATE_8K, 1500, 1024, 2, 0, 257, 2) / 2, 500 );

            // Zoom = 2, center = 1000
            HFftOutput<int16_t> fft_3(1024, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow, 15, H_SAMPLE_RATE_8K, 2, 1000);
            ASSERT_IS_EQUAL(fft_3.Rate(), 4000);
            ASSERT_IS_EQUAL(fft_3.Width(), 2000);
            ASSERT_IS_EQUAL(fft_3.Center(), 1000);
            ASSERT_IS_EQUAL(fft_3.Left(), 0);
            ASSERT_IS_EQUAL(fft_3.Right(), 2000);
            ASSERT_IS_EQUAL((int) (fft_3.BinSize() * 100000), 390625);
            ASSERT_IS_EQUAL( test_fft_rate_frequency_N(H_SAMPLE_RATE_8K, 800, 1024, 2, 1000, 257, 2) / 2, 800 );

            // Zoom = 4, center = 2500
            HFftOutput<int16_t> fft_4(1024, 1, new HCustomWriter<HFftResults>(staticCallback), new TestWindow, 15, H_SAMPLE_RATE_8K, 4, 2500);
            ASSERT_IS_EQUAL(fft_4.Rate(), 2000);
            ASSERT_IS_EQUAL(fft_4.Width(), 1000);
            ASSERT_IS_EQUAL(fft_4.Center(), 2500);
            ASSERT_IS_EQUAL(fft_4.Left(), 2000);
            ASSERT_IS_EQUAL(fft_4.Right(), 3000);
            ASSERT_IS_EQUAL((int) (fft_4.BinSize() * 100000), 195312);
            ASSERT_IS_EQUAL( test_fft_rate_frequency_N(H_SAMPLE_RATE_8K, 2800, 1024, 4, 2500, 257, 4) / 4, 800 );
        }

} hfftoutput_test;

int HFftOutput_Test::staticReady = 0;

double* HFftOutput_Test::staticSpectrum;
