#include <stdio.h>
#include <iostream>

#include "test.h"

class HIqFirDecimator_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_read);
            UNITTEST(test_write);
            UNITTEST(test_writerConsumer);

            UNITTEST(test_read_dont_collect);
            UNITTEST(test_write_dont_collect);
            UNITTEST(test_writerConsumer_dont_collect);
        }

        const char* name()
        {
            return "HIqFirDecimator";
        }

    private:

        void test_read()
        {
            int8_t input[] = {1, 12, 2, 11, 3, 10, 4, 9, 5, 8, 6, 7, 7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12, 1};
            int8_t expected[] = {1, 12, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestReader<int8_t> rd(input, 24);

            HIqFirDecimator<int8_t> fdm(rd.Reader(), 4, coefs, 5, 24);
            int8_t received[24];

            ASSERT_IS_EQUAL(fdm.Read(received, 24), 24);
            ASSERT_IS_EQUAL(rd.Reads, 4);
            ASSERT_IS_EQUAL(rd.Samples, 96);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 24), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_write()
        {
            int8_t input[] = {1, 12, 2, 11, 3, 10, 4, 9, 5, 8, 6, 7, 7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12, 1};
            int8_t expected[] = {1, 12, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> wr(24);

            HIqFirDecimator<int8_t> fdm(wr.Writer(), 4, coefs, 5, 24);
            ASSERT_IS_EQUAL(fdm.Write(input, 24), 24);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(fdm.Write(input, 24), 24);
            ASSERT_IS_EQUAL(fdm.Write(input, 24), 24);
            ASSERT_IS_EQUAL(fdm.Write(input, 24), 24);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 24);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 24), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_writerConsumer()
        {
            int8_t input[] = {1, 12, 2, 11, 3, 10, 4, 9, 5, 8, 6, 7, 7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12, 1};
            int8_t expected[] = {1, 12, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> srcWr(24);

            HIqFirDecimator<int8_t> fdm(srcWr.Consumer(), 4, coefs, 5, 24);
            TestWriter<int8_t> wr(fdm.Consumer(), 24);

            ASSERT_IS_EQUAL(srcWr.Write(input, 24), 24);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(srcWr.Write(input, 24), 24);
            ASSERT_IS_EQUAL(srcWr.Write(input, 24), 24);
            ASSERT_IS_EQUAL(srcWr.Write(input, 24), 24);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 24);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 24), 0);

            ASSERT_IS_TRUE(srcWr.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_read_dont_collect()
        {
            int8_t input[] = {1, 12, 2, 11, 3, 10, 4, 9, 5, 8, 6, 7, 7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12, 1};
            int8_t expected[] = {1, 12, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestReader<int8_t> rd(input, 24);

            HIqFirDecimator<int8_t> fdm(rd.Reader(), 4, coefs, 5, 24, false);

            int8_t received[6];

            ASSERT_IS_EQUAL(fdm.Read(received, 6), 6);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 24);

            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_write_dont_collect()
        {
            int8_t input[] = {1, 12, 2, 11, 3, 10, 4, 9, 5, 8, 6, 7, 7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12, 1};
            int8_t expected[] = {1, 12, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> wr(24);

            HIqFirDecimator<int8_t> fdm(wr.Writer(), 4, coefs, 5, 24, false);

            ASSERT_IS_EQUAL(fdm.Write(input, 24), 24);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 6);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);

            ASSERT_IS_TRUE(fdm.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_writerConsumer_dont_collect()
        {
            int8_t input[] = {1, 12, 2, 11, 3, 10, 4, 9, 5, 8, 6, 7, 7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12, 1};
            int8_t expected[] = {1, 12, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30, 43, 22, 15, 50, 35, 30};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> srcWr(24);

            HIqFirDecimator<int8_t> fdm(srcWr.Consumer(), 4, coefs, 5, 24, false);
            TestWriter<int8_t> wr(fdm.Consumer(), 6);

            ASSERT_IS_EQUAL(srcWr.Write(input, 24), 24);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 6);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);

            ASSERT_IS_TRUE(srcWr.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

} hiqfirdecimator_test;