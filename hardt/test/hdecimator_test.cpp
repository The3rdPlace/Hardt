#include <iostream>

#include "test.h"

class HDecimator_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_read);
            UNITTEST(test_write);
            UNITTEST(test_writerConsumer);

            UNITTEST(test_read_dont_collect);
            UNITTEST(test_write_dont_collect);
            UNITTEST(test_writerConsumer_dont_collect);

            UNITTEST(test_read_3);
            UNITTEST(test_write_3);

            UNITTEST(test_read_5);
            UNITTEST(test_write_5);

            UNITTEST(test_read_with_start);
            UNITTEST(test_write_with_start);
            UNITTEST(test_writerConsumer_with_start);
        }

        const char* name() override
        {
            return "HDecimator";
        }

    private:

        void test_read()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            int8_t expected2[] = {12, 10, 8, 6, 4, 2, 12, 10, 8, 6, 4, 2};
            TestReader<int8_t> rd("hdecimator_test_testreader", input, 48, true, true);

            HDecimator<int8_t> dm("hdecimator_test_read", rd.Reader(), 2, 12);

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
            TestWriter<int8_t> wr("hdecimator_test_testwriter", 12);

            HDecimator<int8_t> dm("hdecimator_test_write", wr.Writer(), 2, 12);

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
            TestWriter<int8_t> srcWr("hdecimator_test_testwriter_src", 12);

            HDecimator<int8_t> dm("hdecimator_test_writerconsumer", srcWr.Consumer(), 2, 12);
            TestWriter<int8_t> wr("hdecimator_test_testwriter_wr", dm.Consumer(), 12);

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

        void test_read_dont_collect()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            TestReader<int8_t> rd("hdecimator_test_testreader", input, 48, true, true);

            HDecimator<int8_t> dm("hdecimator_test_read_dont_collect", rd.Reader(), 2, 12, false);

            int8_t received[12];

            ASSERT_IS_EQUAL(dm.Read(received, 6), 6);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected1, 6), 0);
        }

        void test_write_dont_collect()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            TestWriter<int8_t> wr("hdecimator_test_testwriter", 12);

            HDecimator<int8_t> dm("hdecimator_test_write_dont_collect", wr.Writer(), 2, 12, false);

            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected1, 6), 0);
        }

        void test_writerConsumer_dont_collect()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected1[] = {1, 3, 5, 7, 9, 11, 1, 3, 5, 7, 9, 11};
            TestWriter<int8_t> srcWr("hdecimator_test_testwriter_src", 12);

            HDecimator<int8_t> dm("hdecimator_test_writerconsumer_dont_collect", srcWr.Consumer(), 2, 12, false);
            TestWriter<int8_t> wr("hdecimator_test_testwriter_wr", dm.Consumer(), 6);

            ASSERT_IS_EQUAL(srcWr.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected1, 6), 0);
        }

        void test_read_3()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected1[] = {1, 4, 7, 10, 1, 4, 7, 10, 12, 9, 6, 3};
            int8_t expected2[] = {12, 9, 6, 3, 1, 4, 7, 10, 1, 4, 7, 10};
            TestReader<int8_t> rd("hdecimator_test_testreader", input, 72, true, true);

            HDecimator<int8_t> dm("hdecimator_test_read_3", rd.Reader(), 3, 12);

            int8_t received[12];

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 3);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected2, 12), 0);
        }

        void test_write_3()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t input2[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 4, 7, 10, 1, 4, 7, 10, 1, 4, 7, 10};
            int8_t expected2[] = {12, 9, 6, 3, 12, 9, 6, 3, 12, 9, 6, 3};
            TestWriter<int8_t> wr("hdecimator_test_testwriter", 12);

            HDecimator<int8_t> dm("hdecimator_test_write_3", wr.Writer(), 3, 12);

            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected2, 12), 0);
        }

        void test_read_5()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected1[] = {1, 6, 11, 4, 9, 2, 7, 12, 5, 10, 3, 8};
            int8_t expected2[] = {1, 6, 11, 4, 9, 2, 7, 12, 5, 10, 3, 8};
            TestReader<int8_t> rd("hdecimator_test_testreader", input, 12);

            HDecimator<int8_t> dm("hdecimator_test_read_5", rd.Reader(), 5, 12);

            int8_t received[12];

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 10);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected2, 12), 0);
        }

        void test_write_5()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t input2[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {1, 6, 11, 4, 9, 2, 7, 12, 5, 10, 3, 8};
            int8_t expected2[] = {12, 7, 2, 9, 4, 11, 6, 1, 8, 3, 10, 5};
            TestWriter<int8_t> wr("hdecimator_test_testwriter", 12);

            HDecimator<int8_t> dm("hdecimator_test_write_5", wr.Writer(), 5, 12);

            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(dm.Write(input1, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(dm.Write(input2, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected2, 12), 0);
        }

        void test_read_with_start()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12};
            int8_t expected2[] = {11, 9, 7, 5, 3, 1, 11, 9, 7, 5, 3, 1};
            TestReader<int8_t> rd("hdecimator_test_testreader", input, 48, true, true);

            HDecimator<int8_t> dm("hdecimator_test_read_with_start", rd.Reader(), 2, 12, true, 1);

            int8_t received[12];

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected1, 12), 0);

            ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 4);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected2, 12), 0);
        }

        void test_write_with_start()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t input2[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12};
            int8_t expected2[] = {11, 9, 7, 5, 3, 1, 11, 9, 7, 5, 3, 1};
            TestWriter<int8_t> wr("hdecimator_test_testwriter", 12);

            HDecimator<int8_t> dm("hdecimator_test_write_with_start", wr.Writer(), 2, 12, true, 1);

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

        void test_writerConsumer_with_start()
        {
            int8_t input1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t input2[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
            int8_t expected1[] = {2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12};
            int8_t expected2[] = {11, 9, 7, 5, 3, 1, 11, 9, 7, 5, 3, 1};
            TestWriter<int8_t> srcWr("hdecimator_test_testwriter_src", 12);

            HDecimator<int8_t> dm("hdecimator_test_writerconsumer_with_start", srcWr.Consumer(), 2, 12, true, 1);
            TestWriter<int8_t> wr("hdecimator_test_testwriter_wr", dm.Consumer(), 12);

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

} hdecimator_test;