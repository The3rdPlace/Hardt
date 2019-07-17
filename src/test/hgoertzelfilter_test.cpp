#include <stdio.h>
#include <iostream>

#include "test.h"

class HGoertzelFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_goertzel);
        }

        const char* name()
        {
            return "HGoertzelFilter";
        }

    private:

        int magnitude = -1;

        int callback(int* data, size_t length)
        {
            magnitude = *data;
            return length;
        }

        void test_goertzel()
        {
            const int N = 1024;

            // Get a 1KHz sinewave generator at samplerate 48K
            HSineGenerator<int16_t> g(48000, 1000, 100);
            int16_t input[4 * N];
            g.Read(input, 4 * N);

            HGoertzelFilter<int16_t>* grtzl;
            HRectangularWindow<int16_t> window;

            float n = N;
            for( float bin = 1; bin < N / 2; bin += 0.1f )
            {
                // Check for result at 1KHz
                grtzl = new HGoertzelFilter<int16_t>(N, 4, bin, HCustomWriter<int>::Create<HGoertzelFilter_Test>(this, &HGoertzelFilter_Test::callback), &window);
                ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);

                if( floor(bin * 10) == 213 ) // = bin 21.3 adjusted for tiny decimal fractions
                {
                    if( ! ASSERT_IS_EQUAL(magnitude, 51301) )
                    {
                        std::cout << "At bin = " << bin << std::endl;
                        break;
                    }
                }
                else
                {
                    if (! ASSERT_IS_LESS_THAN(magnitude, 51301) )
                    {
                        std::cout << "At bin = " << bin << std::endl;
                        break;
                    }
                }
                delete grtzl;
                magnitude = -1;
            }
        }

} hgoertzelfilter_test;
