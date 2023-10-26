#include <iostream>

#include "test.h"

class HIq2IConverter_Test: public Test
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
            return "HIq2IConverter";
        }

    private:

        void test_read()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            int8_t expected2[] = {12, 10, 8, 6, 4, 2, 12, 10, 8, 6, 4, 2};
            TestReader<int8_t> rd("hiq2iconverter_test_testreader", input, 48, true, true);

            HIq2IConverter<int8_t> dm("hiq2iconverter_test_read", rd.Reader(), 12);

            int8_t received[12];

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 4);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected2, 12), 0);
        }

        void test_write()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t input2[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            int8_t expected2[] = {12, 10, 8, 6, 4, 2, 12, 10, 8, 6, 4, 2};
            TestWriter<int8_t> wr("hiq2iconverter_test_testwriter", 12);

            HIq2IConverter<int8_t> dm("hiq2iconverter_test_write", wr.Writer(), 12);

            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected2, 12), 0);
        }

        void test_writerConsumer()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t input2[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            int8_t expected2[] = {12, 10, 8, 6, 4, 2, 12, 10, 8, 6, 4, 2};
            TestWriter<int8_t> srcWr("hiq2iconverter_test_testwriter_src", 12);

            HIq2IConverter<int8_t> dm("hiq2iconverter_test_writerconsumer", srcWr.Consumer(), 12);
            TestWriter<int8_t> wr("hiq2iconverter_test_testwriter_wr", dm.Consumer(), 12);

            ASSERT_IS_EQUAL(srcWr.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(srcWr.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(srcWr.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(srcWr.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected2, 12), 0);
        }

} hiq2iconverter_test;