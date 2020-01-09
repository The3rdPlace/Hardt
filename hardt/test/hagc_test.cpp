#include <stdio.h>
#include <iostream>

#include "test.h"

class HAgc_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_agc_as_writer);
            UNITTEST(test_agc_as_writer_with_lock_and_hold);
        }

        const char* name()
        {
            return "Hagc";
        }

    private:

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int8_t received[6];

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) received, src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_agc_as_writer()
        {
            TestWriter<int8_t> wr;
            int8_t input_low[6] = {1, 1, 1, 1, 1, 1};
            int8_t input_high[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_2[6] = {2, 2, 2, 2, 2, 2};
            int8_t expected_7[6] = {7, 7, 7, 7, 7 ,7};
            int8_t expected_8[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_56[6] = {56, 56, 56, 56, 56, 56};
            int8_t expected_16[6] = {16, 16, 16, 16, 16, 16};
            HAgc<int8_t> agc(&wr, 8, 8, 1, 0, 6);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_2, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_7, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_8, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_8, 6 * sizeof(int8_t)), 0);

            // Now send in a strong signal
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_56, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_16, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_8, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_high, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_8, 6 * sizeof(int8_t)), 0);
        }

        void test_agc_as_writer_with_lock_and_hold()
        {
            TestWriter<int8_t> wr;
            int8_t input_low[6] = {1, 1, 1, 1, 1, 1};
            int8_t input_mid[6] = {2, 2, 2, 2, 2, 2};
            int8_t input_high[6] = {3, 3, 3, 3, 3, 3};
            int8_t expected_4[6] = {4, 4, 4, 4, 4, 4};
            int8_t expected_8[6] = {8, 8, 8, 8, 8, 8};
            int8_t expected_14[6] = {14, 14, 14, 14, 14 ,14};
            int8_t expected_16[6] = {16, 16, 16, 16, 16, 16};
            int8_t expected_56[6] = {56, 56, 56, 56, 56, 56};
            HAgc<int8_t> agc(&wr, 16, 16, 1, 2, 6);

            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_4, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_14, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_16, 6 * sizeof(int8_t)), 0);

            // Keep writing and make sure the lock is holding
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_mid, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_16, 6 * sizeof(int8_t)), 0);

            // Now write in a low signal, check that the lock is holding for 2+1 blocks
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_8, 6 * sizeof(int8_t)), 0);

            // Keep writing a low signal, the lock should now release and increase the gain
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(agc.Write(input_low, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_14, 6 * sizeof(int8_t)), 0);
        }
} hagc_test;