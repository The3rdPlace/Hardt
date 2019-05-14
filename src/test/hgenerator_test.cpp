#include <stdio.h>
#include <iostream>

#include "test.h"

class HGenerator_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_sine_10_100);
            UNITTEST(test_cosine_10_100);
        }

        const char* name()
        {
            return "HGenerator";
        }

    private:

        class test_generator : public HGenerator<int8_t>
        {
            public:

                test_generator(int rate, int frequency, float phase = 0):
                    HGenerator<int8_t>((H_SAMPLE_RATE) rate, frequency, 127, phase)
                {}
        };

        void test_sine_10_100()
        {
            int rate = 100;
            int frequency = 10;

            test_generator tg(rate, frequency);

            int8_t buffer[11];
            tg.Read(buffer, 11);

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

        void test_cosine_10_100()
        {
            int rate = 100;
            int frequency = 10;
            float phase = M_PI / 2;
            test_generator tg(rate, frequency, phase);

            int8_t buffer[11];
            tg.Read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], 126);
            ASSERT_IS_EQUAL((int) buffer[1], 102);
            ASSERT_IS_EQUAL((int) buffer[2], 39);
            ASSERT_IS_EQUAL((int) buffer[3], -39);
            ASSERT_IS_EQUAL((int) buffer[4], -102);
            ASSERT_IS_EQUAL((int) buffer[5], -126);
            ASSERT_IS_EQUAL((int) buffer[6], -102);
            ASSERT_IS_EQUAL((int) buffer[7], -39);
            ASSERT_IS_EQUAL((int) buffer[8], 39);
            ASSERT_IS_EQUAL((int) buffer[9], 102);
            ASSERT_IS_EQUAL((int) buffer[10], 126);
        }
} hgenerator_test;