#include <iostream>

#include "test.h"

class HFirDecimator_Test: public Test
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
        }

        const char* name() override
        {
            return "HFirDecimator";
        }

    private:

        void test_read()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestReader<int8_t> rd("hfirdecimator_test_testreader", input, 24);

            HFirDecimator<int8_t> fdm("hfirdecimator_test_read", rd.Reader(), 4, coefs, 5, 12);

            int8_t received[12];

            ASSERT_IS_EQUAL(fdm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 4);

            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_write()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> wr("hfirdecimator_test_testwriter", 12);

            HFirDecimator<int8_t> fdm("hfirdecimator_test_write", wr.Writer(), 4, coefs, 5, 12);

            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_writerConsumer()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> srcWr("hfirdecimator_test_testwriter_src", 12);

            HFirDecimator<int8_t> fdm("hfirdecimator_test_writerconsumer", srcWr.Consumer(), 4, coefs, 5, 12);
            TestWriter<int8_t> wr("hfirdecimator_test_testwriter_wr", fdm.Consumer(), 12);

            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);

            ASSERT_IS_TRUE(srcWr.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_read_dont_collect()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestReader<int8_t> rd("hfirdecimator_test_testreader", input, 24);

            HFirDecimator<int8_t> fdm("hfirdecimator_test_read_dont_collect", rd.Reader(), 4, coefs, 5, 12, false);

            int8_t received[3];

            ASSERT_IS_EQUAL(fdm.Read(received, 3), 3);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 12);

            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 3), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_write_dont_collect()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> wr("hfirdecimator_test_testwriter", 12);

            HFirDecimator<int8_t> fdm("hfirdecimator_test_write_dont_collect", wr.Writer(), 4, coefs, 5, 12, false);

            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 3);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 3), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_writerConsumer_dont_collect()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> srcWr("hfirdecimator_test_testwriter_src", 12);

            HFirDecimator<int8_t> fdm("hfirdecimator_test_writerconsumer_dont_collect", srcWr.Consumer(), 4, coefs, 5, 12, false);
            TestWriter<int8_t> wr("hfirdecimator_test_testwriter_wr", fdm.Consumer(), 3);

            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 3);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 3), 0);

            ASSERT_IS_TRUE(srcWr.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

} hfirdecimator_test;