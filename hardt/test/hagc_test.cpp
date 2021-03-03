#include <stdio.h>
#include <iostream>

#include "test.h"

class HAgc_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_agc_as_writer);
            UNITTEST(test_agc_as_writer_with_allow_unity);
        }

        const char* name()
        {
            return "Hagc";
        }

    private:

        void test_agc_as_writer()
        {
            TestWriter<int8_t> wr(6);
            int8_t input_low[6] = {1, 1, 1, 1, 1, 1};
            int8_t input_high[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_2[6] = {1, 1, 1, 1, 1, 1};
            int8_t expected_7[6] = {8, 8, 8, 8, 8 ,8};
            int8_t expected_8[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_10[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_12[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_17[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_18[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_20[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_26[6] = {8, 8, 8, 8, 8, 8};
            HAgc<int8_t> agc(wr.Writer(), 8, 2, 6);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_2, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_7, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_8, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_10, 6 * sizeof(int8_t)), 0);

            // Now send in a strong signal
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_12, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_17, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_18, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_20, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_26, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(agc.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_agc_as_writer_with_allow_unity()
        {
            TestWriter<int8_t> wr(6);
            int8_t input_low[6] = {1, 1, 1, 1, 1, 1};
            int8_t input_high[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_2[6] = {1, 1, 1, 1, 1, 1};
            int8_t expected_7[6] = {8, 8, 8, 8, 8 ,8};
            int8_t expected_8[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_10[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_12[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_17[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_18[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_20[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_26[6] = {16, 16, 16, 16, 16, 16};
            HAgc<int8_t> agc(wr.Writer(), 8, 2, 6, true);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_2, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_7, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_8, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_10, 6 * sizeof(int8_t)), 0);

            // Now send in a strong signal
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_12, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_17, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_18, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_20, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_26, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(agc.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }
} hagc_test;