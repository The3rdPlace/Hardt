#include <iostream>

#include "test.h"

class HUpsampler_Test: public Test
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
            return "HUpsampler";
        }

    private:

        void test_read()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[32] = {1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0};
            TestReader<int8_t> rd("hupsampler_test_testreader", input, 8);

            HUpsampler<int8_t> us("hupsampler_test_read", rd.Reader(), 4, 8);

            int8_t received[32];

            ASSERT_IS_EQUAL(us.Read(received, 8), 8);
            ASSERT_IS_EQUAL(us.Read(&received[8], 8), 8);
            ASSERT_IS_EQUAL(us.Read(&received[16], 8), 8);
            ASSERT_IS_EQUAL(us.Read(&received[24], 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 32), 0);
        }

        void test_write()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[32] = {1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0};
            TestWriter<int8_t> wr("hupsampler_test_testwriter", 32, true, 0, true);

            HUpsampler<int8_t> us("hupsampler_test_write", wr.Writer(), 4, 8);

            ASSERT_IS_EQUAL(us.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 4);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 32), 0);
        }

        void test_writerConsumer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[32] = {1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0};
            TestWriter<int8_t> srcWr("hupsampler_test_testwriter_src", 8);

            HUpsampler<int8_t> us("hupsampler_test_writerconsumer", srcWr.Consumer(), 4, 8);
            TestWriter<int8_t> wr("hupsampler_test_testwriter_wr", us.Consumer(), 32, true, 0, true);

            ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 4);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 32), 0);
        }

} hupsampler_test;