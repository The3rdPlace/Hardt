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

            // Check for result at 1KHz
            int bin = (24000 / 1024) * 1000;
            grtzl = new HGoertzelFilter<int16_t>(N, 4, bin, HCustomWriter<int>::Create<HGoertzelFilter_Test>(this, &HGoertzelFilter_Test::callback), &window);
            ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);
            ASSERT_IS_EQUAL(magnitude, 4); // Todo: Check for real expected value
            delete grtzl;
            magnitude = -1;

            // Check for result at 1KHz + 1 bin
            bin = ((24000 / 1024) * 1000) + 1;
            grtzl = new HGoertzelFilter<int16_t>(N, 4, bin, HCustomWriter<int>::Create<HGoertzelFilter_Test>(this, &HGoertzelFilter_Test::callback), &window);
            ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);
            ASSERT_IS_EQUAL(magnitude, 4); // Todo: Check for real expected value
            delete grtzl;
            magnitude = -1;

            // Check for result at 1KHz - 1 bin
            bin = ((24000 / 1024) * 1000) - 1;
            grtzl = new HGoertzelFilter<int16_t>(N, 4, bin, HCustomWriter<int>::Create<HGoertzelFilter_Test>(this, &HGoertzelFilter_Test::callback), &window);
            ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);
            ASSERT_IS_EQUAL(magnitude, 4); // Todo: Check for real expected value
            delete grtzl;
            magnitude = -1;

            // Check for result at 500Hz
            bin = (24000 / 1024) * 500;
            grtzl = new HGoertzelFilter<int16_t>(N, 4, bin, HCustomWriter<int>::Create<HGoertzelFilter_Test>(this, &HGoertzelFilter_Test::callback), &window);
            ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);
            ASSERT_IS_EQUAL(magnitude, 4); // Todo: Check for real expected value
            delete grtzl;
            magnitude = -1;

            // Check for result at 4KHz
            bin = (24000 / 1024) * 4000;
            grtzl = new HGoertzelFilter<int16_t>(N, 4, bin, HCustomWriter<int>::Create<HGoertzelFilter_Test>(this, &HGoertzelFilter_Test::callback), &window);
            ASSERT_IS_EQUAL(grtzl->Write(input, 4 * N), 4 * N);
            ASSERT_IS_EQUAL(magnitude, 4); // Todo: Check for real expected value
            delete grtzl;
            magnitude = -1;
        }

} hgoertzelfilter_test;
