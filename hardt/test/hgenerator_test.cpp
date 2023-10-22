#include <iostream>
#include <utility>

#include "test.h"

class HGenerator_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_sine_10_100);
            UNITTEST(test_cosine_10_100);
            UNITTEST(test_cosine_10_100_from_next);
        }

        const char* name() override
        {
            return "HGenerator";
        }

    private:

        class test_generator : public HGenerator<int8_t>
        {
            public:

                test_generator(std::string id, int rate, int frequency, float phase = 0):
                    HGenerator<int8_t>(std::move(id), (H_SAMPLE_RATE) rate, frequency, 127, phase)
                {}
        };

        void test_sine_10_100()
        {
            int rate = 100;
            int frequency = 10;

            test_generator tg("test_generator", rate, frequency);

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
            test_generator tg("test_generator", rate, frequency, phase);

            int8_t buffer[11];
            tg.Read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], 127);
            ASSERT_IS_EQUAL((int) buffer[1], 102);
            ASSERT_IS_EQUAL((int) buffer[2], 39);
            ASSERT_IS_EQUAL((int) buffer[3], -39);
            ASSERT_IS_EQUAL((int) buffer[4], -102);
            ASSERT_IS_EQUAL((int) buffer[5], -127);
            ASSERT_IS_EQUAL((int) buffer[6], -102);
            ASSERT_IS_EQUAL((int) buffer[7], -39);
            ASSERT_IS_EQUAL((int) buffer[8], 39);
            ASSERT_IS_EQUAL((int) buffer[9], 102);
            ASSERT_IS_EQUAL((int) buffer[10], 127);
        }

    void test_cosine_10_100_from_next()
    {
        int rate = 100;
        int frequency = 10;
        float phase = M_PI / 2;
        test_generator tg("test_generator", rate, frequency, phase);

        int8_t buffer[11];
        for(signed char & i : buffer) {
            i = tg.Next();
        }

        ASSERT_IS_EQUAL((int) buffer[0], 127);
        ASSERT_IS_EQUAL((int) buffer[1], 102);
        ASSERT_IS_EQUAL((int) buffer[2], 39);
        ASSERT_IS_EQUAL((int) buffer[3], -39);
        ASSERT_IS_EQUAL((int) buffer[4], -102);
        ASSERT_IS_EQUAL((int) buffer[5], -127);
        ASSERT_IS_EQUAL((int) buffer[6], -102);
        ASSERT_IS_EQUAL((int) buffer[7], -39);
        ASSERT_IS_EQUAL((int) buffer[8], 39);
        ASSERT_IS_EQUAL((int) buffer[9], 102);
        ASSERT_IS_EQUAL((int) buffer[10], 127);
    }

} hgenerator_test;