#include <iostream>

#include "test.h"

class HGoertzelOutput_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_goertzel_magnitude);
            UNITTEST(test_goertzel_bin);
            UNITTEST(test_goertzel_phase);
        }

        const char* name() override
        {
            return "HGoertzelOutput";
        }

    private:

        int magnitude = -1;
        int phase = -1;

        int callback(HGoertzelResult* result, size_t length)
        {
            magnitude = (*result).Magnitude;
            phase = (*result).Phase;
            return length;
        }

        void test_goertzel_magnitude()
        {
            const int N = 1024;

            // Get a 1KHz sinewave generator at samplerate 48K
            HSineGenerator<int16_t> g("hgoertzeloutput_test_sine_generator", 48000, 1000, 100);
            int16_t input[4 * N];
            g.Read(input, 4 * N);

            HGoertzelOutput<int16_t>* grtzl;
            HRectangularWindow<int16_t> window;

            float n = N;
            for( float bin = 1; bin < N / 2; bin += 0.1f )
            {
                // Check for result at 1KHz
                grtzl = new HGoertzelOutput<int16_t>("hgoertzeloutput_test_magnitude", N, 4, bin, HCustomWriter<HGoertzelResult>::Create<HGoertzelOutput_Test>("hgoertzeloutput_test_magnitude_custom_writer", this, &HGoertzelOutput_Test::callback), &window);
                ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);

                if( floor(bin * 10) == 213 ) // = bin 21.3 adjusted for tiny decimal fractions
                {
                    if( ! ASSERT_IS_EQUAL(magnitude, 50948) )
                    {
                        std::cout << "At bin = " << bin << std::endl;
                        break;
                    }
                }
                else
                {
                    if (! ASSERT_IS_LESS_THAN(magnitude, 51402) )
                    {
                        std::cout << "At bin = " << bin << std::endl;
                        break;
                    }
                }
                delete grtzl;
                magnitude = -1;
            }
        }

        void test_goertzel_bin()
        {
            const int N = 1024;

            // Get a 1KHz sinewave generator at samplerate 48K
            HSineGenerator<int16_t> g("hgoertzeloutput_test_sine_generator", 48000, 1000, 100);
            int16_t input[4 * N];
            g.Read(input, 4 * N);

            // Get a Goertzel filter at 1Khz (bin = 21.3)
            HRectangularWindow<int16_t> window;
            HGoertzelOutput<int16_t> grtzl("hgoertzeloutput_test_bin", N, 4, 48000, 1000, HCustomWriter<HGoertzelResult>::Create<HGoertzelOutput_Test>("hgoertzeloutput_test_bin_custom_writer", this, &HGoertzelOutput_Test::callback), &window);

            // Check for result at 1KHz
            ASSERT_IS_EQUAL(grtzl.Write(input, 4 * N), 4 * N);
            ASSERT_IS_EQUAL(magnitude, 51034);
        }

        void test_goertzel_phase()
        {
            const int N = 1024;
            const int Iter = 4;

            // Get a 1KHz sine and cosine generator at samplerate 48K
            // It has not importance to use sin and cos, other than that
            // we know that they have a phase difference of 90 degr.,
            HSineGenerator<int16_t> sinGenerator("hgoertzeloutput_test_sine_generator", 48000, 1000, 100);
            HCosineGenerator<int16_t> cosGenerator("hgoertzeloutput_test_cos_generator", 48000, 1000, 100);
            int16_t buffer[N * Iter];

            HRectangularWindow<int16_t> window;
            HGoertzelOutput<int16_t> grtzl("hgoertzeloutput_test_phase", N, Iter, 21.3f, HCustomWriter<HGoertzelResult>::Create<HGoertzelOutput_Test>("hgoertzeloutput_test_phase_custom_writer", this, &HGoertzelOutput_Test::callback), &window);;

            // Run several reads and compare phase
            for( int i = 0; i < 4; i++ )
            {
                int sinPhase, cosPhase;

                sinGenerator.Read(buffer, Iter * N);
                grtzl.Write(buffer, Iter * N);
                sinPhase = phase;
                // ---
                cosGenerator.Read(buffer, Iter * N);
                grtzl.Write(buffer, Iter * N);
                cosPhase = phase;

                // Compare phase difference. The diff may come out as either a positive
                // or a negative value, in the latter case we convert the phase to the positive value
                int diff = cosPhase - sinPhase;
                if( diff < 0 )
                {
                    diff += 360;
                }
                ASSERT_IS_BETWEEN(diff, 89, 91);
            }
        }

} hgoertzeloutput_test;
