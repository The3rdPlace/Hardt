#include <iostream>

#include "test.h"

class HSineGenerator_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_sine_10_100);
        }

        const char* name() override
        {
            return "HSineGenerator";
        }

    private:

        void test_sine_10_100()
        {
            int rate = 100;
            int frequency = 10;
            int amplitude = 127;

            HSineGenerator<int8_t> g("hsinegenerator_test_sine_10_100", rate, frequency, amplitude);

            int8_t buffer[11];
            g.Read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], 0);
            ASSERT_IS_EQUAL((int) buffer[1], 74);
            ASSERT_IS_EQUAL((int) buffer[2], 120);
            ASSERT_IS_EQUAL((int) buffer[3], 120);
            ASSERT_IS_EQUAL((int) buffer[4], 74);
            ASSERT_IS_EQUAL((int) buffer[5], 0);
            ASSERT_IS_EQUAL((int) buffer[6], -74);
            ASSERT_IS_EQUAL((int) buffer[7], -120);
            ASSERT_IS_EQUAL((int) buffer[8], -120);
            ASSERT_IS_EQUAL((int) buffer[9], -74);
            ASSERT_IS_EQUAL((int) buffer[10], 0);
        }
} hsinegenerator_test;