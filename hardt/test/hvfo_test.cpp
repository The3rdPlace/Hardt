#include <iostream>

#include "test.h"

class HVfo_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_sine_cosine_10_100);
            UNITTEST(test_frequency_10_20);
        }

        const char* name() override
        {
            return "HVfo";
        }

    private:

        void test_sine_cosine_10_100()
        {
            int rate = 100;
            int frequency = 10;

            HVfo<int8_t> g("hvfo_test_sine_cosine_10_100", rate, frequency, 127);

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

            g.SetFrequency(frequency, M_PI / 2);
            g.Read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], 102);
            ASSERT_IS_EQUAL((int) buffer[1], 39);
            ASSERT_IS_EQUAL((int) buffer[2], -39);
            ASSERT_IS_EQUAL((int) buffer[3], -102);
            ASSERT_IS_EQUAL((int) buffer[4], -127);
            ASSERT_IS_EQUAL((int) buffer[5], -102);
            ASSERT_IS_EQUAL((int) buffer[6], -39);
            ASSERT_IS_EQUAL((int) buffer[7], 39);
            ASSERT_IS_EQUAL((int) buffer[8], 102);
            ASSERT_IS_EQUAL((int) buffer[9], 127);
            ASSERT_IS_EQUAL((int) buffer[10], 102);
        }

        void test_frequency_10_20()
        {
            int rate = 100;

            HVfo<int8_t> g("htest_hvfo_test_frequency_10_20", rate, 10, 127);

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

            g.SetFrequency(20);
            g.Read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], 74);
            ASSERT_IS_EQUAL((int) buffer[1], 120);
            ASSERT_IS_EQUAL((int) buffer[2], 0);
            ASSERT_IS_EQUAL((int) buffer[3], -120);
            ASSERT_IS_EQUAL((int) buffer[4], -74);
            ASSERT_IS_EQUAL((int) buffer[5], 74);
            ASSERT_IS_EQUAL((int) buffer[6], 120);
            ASSERT_IS_EQUAL((int) buffer[7], 0);
            ASSERT_IS_EQUAL((int) buffer[8], -120);
            ASSERT_IS_EQUAL((int) buffer[9], -74);
            ASSERT_IS_EQUAL((int) buffer[10], 74);
        }
} hvfo_test;
