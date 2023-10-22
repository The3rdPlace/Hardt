#include <iostream>

#include "test.h"

class HCosineGenerator_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_cosine_10_100);
        }

        const char* name() override
        {
            return "HCosineGenerator";
        }

    private:

        void test_cosine_10_100()
        {
            int rate = 100;
            int frequency = 10;
            int amplitude = 127;

            HCosineGenerator<int8_t> g("hcosinegenerator_test_cosine_10_100", rate, frequency, amplitude);

            int8_t buffer[11];
            g.Read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], -127);
            ASSERT_IS_EQUAL((int) buffer[1], -102);
            ASSERT_IS_EQUAL((int) buffer[2], -39);
            ASSERT_IS_EQUAL((int) buffer[3], 39);
            ASSERT_IS_EQUAL((int) buffer[4], 102);
            ASSERT_IS_EQUAL((int) buffer[5], 127);
            ASSERT_IS_EQUAL((int) buffer[6], 102);
            ASSERT_IS_EQUAL((int) buffer[7], 39);
            ASSERT_IS_EQUAL((int) buffer[8], -39);
            ASSERT_IS_EQUAL((int) buffer[9], -102);
            ASSERT_IS_EQUAL((int) buffer[10], -127);
        }
} hcosinegenerator_test;