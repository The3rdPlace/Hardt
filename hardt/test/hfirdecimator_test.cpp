#include <stdio.h>
#include <iostream>

#include "test.h"

class HFirDecimator_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_read);
            UNITTEST(test_write);
            UNITTEST(test_writerConsumer);
        }

        const char* name()
        {
            return "HFirDecimator";
        }

    private:

        void test_read()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestReader<int8_t> rd(input, 24);

            HFirDecimator<int8_t> fdm(rd.Reader(), 4, coefs, 5, 12);

            int8_t received[12];

            ASSERT_IS_EQUAL(fdm.Read(received, 12), 12);
            ASSERT_IS_EQUAL(rd.Reads, 4);

            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
        }

        void test_write()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> wr(12);

            HFirDecimator<int8_t> fdm(wr.Writer(), 4, coefs, 5, 12);

            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(fdm.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);
        }

        void test_writerConsumer()
        {
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            int8_t expected[] = {1, 15, 35, 43, 15, 35, 43, 15, 35, 43, 15, 35};
            float coefs[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
            TestWriter<int8_t> srcWr(12);

            HFirDecimator<int8_t> fdm(srcWr.Consumer(), 4, coefs, 5, 12);
            TestWriter<int8_t> wr(fdm.Consumer(), 12);

            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 0);
            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
            ASSERT_IS_EQUAL(wr.Writes, 1);

            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);
        }

} hfirdecimator_test;