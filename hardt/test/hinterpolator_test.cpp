#include <iostream>

#include "test.h"

class HInterpolator_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_read);
            UNITTEST(test_write);
            UNITTEST(test_writerConsumer);
        }

        const char* name() override
        {
            return "HInterpolator";
        }

    private:

        void test_read()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[32] = {1, 2, 3, 2, 3, 6, 9, 6, 6, 12, 18, 12, 9, 18, 27, 18, 12, 24, 36, 24, 15, 30, 45, 30, 18, 36, 54, 36, 21, 42, 63, 42};
            TestReader<int8_t> rd("hinterpolator_test_testreader", input, 8);

            float filter[12] = {1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2 };
            HInterpolator<int8_t> ip("hinterpolator_test_read", rd.Reader(), 4, filter, 12, 8);

            int8_t received[32];

            ASSERT_IS_EQUAL(ip.Read(received, 8), 8);
            ASSERT_IS_EQUAL(ip.Read(&received[8], 8), 8);
            ASSERT_IS_EQUAL(ip.Read(&received[16], 8), 8);
            ASSERT_IS_EQUAL(ip.Read(&received[24], 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 32), 0);
        }

        void test_write()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[32] = {1, 2, 3, 2, 3, 6, 9, 6, 6, 12, 18, 12, 9, 18, 27, 18, 12, 24, 36, 24, 15, 30, 45, 30, 18, 36, 54, 36, 21, 42, 63, 42};
            TestWriter<int8_t> wr("hinterpolator_test_testwriter", 32, true, 0, true);

            float filter[12] = {1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2 };
            HInterpolator<int8_t> ip("hinterpolator_test_write", wr.Writer(), 4, filter, 12, 8);

            ASSERT_IS_EQUAL(ip.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 4);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 32), 0);
        }

        void test_writerConsumer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[32] = {1, 2, 3, 2, 3, 6, 9, 6, 6, 12, 18, 12, 9, 18, 27, 18, 12, 24, 36, 24, 15, 30, 45, 30, 18, 36, 54, 36, 21, 42, 63, 42};
            TestWriter<int8_t> srcWr("hinterpolator_test_testwriter_src", 8);

            float filter[12] = {1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2 };
            HInterpolator<int8_t> ip("hinterpolator_test_writerconsumer", srcWr.Consumer(), 4, filter, 12, 8);
            TestWriter<int8_t> wr("hinterpolator_test_testwriter_wr", ip.Consumer(), 32, true, 0, true);

            ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 4);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 32), 0);
        }

} hinterpolator_test;