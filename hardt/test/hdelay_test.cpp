#include <iostream>

#include "test.h"

class HDelay_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_delay_with_seconds);
            UNITTEST(test_delay_with_blocks);
        }

        const char* name() override
        {
            return "HDelay";
        }

    private:

        void test_delay_with_seconds()
        {
            TestWriter<int8_t> wr("hdelay_test_testwriter", 6);
            int8_t input_even[6] = {1, 1, 1, 1, 1, 1};
            int8_t input_odd[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_0[6] = {0, 0, 0, 0, 0, 0};
            int8_t expected_even[6] = {1, 1, 1, 1, 1, 1};
            int8_t expected_odd[6] = {16, 16, 16, 16, 16, 16};
            HDelay<int8_t> agc("hdelay_test_delay_with_seconds", wr.Writer(), 6, 100, 2);

            // Run 2 seconds á 100 samples per seconds at blocksize=6 through the filter
            //   100 / 6 = 16.67 ~= 17 blocks per second
            //   2 seconds = 2* 17 = 34 blocks
            for( int i = 0; i < 34; i += 2 ) {
                ASSERT_IS_EQUAL(agc.Write(input_even, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_0, 6 * sizeof(int8_t)), 0);
                ASSERT_IS_EQUAL(agc.Write(input_odd, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_0, 6 * sizeof(int8_t)), 0);
            }

            // Run two more blocks through the filter, now we should start seing input values
            ASSERT_IS_EQUAL(agc.Write(input_even, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_even, 6 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(agc.Write(input_odd, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_odd, 6 * sizeof(int8_t)), 0);

            // Run 4 seconds through the filter to make sure positions change correctly
            for( int i = 0; i < 68; i += 2 ) {
                ASSERT_IS_EQUAL(agc.Write(input_even, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_even, 6 * sizeof(int8_t)), 0);
                ASSERT_IS_EQUAL(agc.Write(input_odd, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_odd, 6 * sizeof(int8_t)), 0);
            }

            ASSERT_IS_TRUE(agc.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_delay_with_blocks()
        {
            TestWriter<int8_t> wr("hdelay_test_testwriter", 6);
            int8_t input_even[6] = {1, 1, 1, 1, 1, 1};
            int8_t input_odd[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_0[6] = {0, 0, 0, 0, 0, 0};
            int8_t expected_even[6] = {1, 1, 1, 1, 1, 1};
            int8_t expected_odd[6] = {16, 16, 16, 16, 16, 16};
            HDelay<int8_t> agc("hdelay_test_delay_with_blocks", wr.Writer(), 6, 34);

            // Run 2 seconds á 100 samples per seconds at blocksize=6 through the filter
            //   100 / 6 = 16.67 ~= 17 blocks per second
            //   2 seconds = 2* 17 = 34 blocks
            for( int i = 0; i < 34; i += 2 ) {
                ASSERT_IS_EQUAL(agc.Write(input_even, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_0, 6 * sizeof(int8_t)), 0);
                ASSERT_IS_EQUAL(agc.Write(input_odd, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_0, 6 * sizeof(int8_t)), 0);
            }

            // Run two more blocks through the filter, now we should start seing input values
            ASSERT_IS_EQUAL(agc.Write(input_even, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_even, 6 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(agc.Write(input_odd, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_odd, 6 * sizeof(int8_t)), 0);

            // Run 4 seconds through the filter to make sure positions change correctly
            for( int i = 0; i < 68; i += 2 ) {
                ASSERT_IS_EQUAL(agc.Write(input_even, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_even, 6 * sizeof(int8_t)), 0);
                ASSERT_IS_EQUAL(agc.Write(input_odd, 6), 6);
                ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_odd, 6 * sizeof(int8_t)), 0);
            }

            ASSERT_IS_TRUE(agc.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

} HDelay_test;