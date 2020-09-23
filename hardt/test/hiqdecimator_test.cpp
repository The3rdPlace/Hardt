#include <stdio.h>
#include <iostream>

#include "test.h"

class HIqDecimator_Test: public Test
{
public:

    void run()
    {
        TEST(test_read);
        TEST(test_write);
        TEST(test_writerConsumer);

        TEST(test_read_dont_collect);
        TEST(test_write_dont_collect);
        TEST(test_writerConsumer_dont_collect);

        TEST(test_read_3);
        TEST(test_write_3);

        TEST(test_read_5);
        TEST(test_write_5);
    }

    const char* name()
    {
        return "HIqDecimator";
    }

private:

    void test_read()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 1, 2, 5, 6, 9, 10};
        TestReader<int8_t> rd(input, 12);

        HIqDecimator<int8_t> dm(rd.Reader(), 2, 12);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
        ASSERT_IS_EQUAL(rd.Reads, 2);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
    }

    void test_write()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 1, 2, 5, 6, 9, 10};
        TestWriter<int8_t> wr(12);

        HIqDecimator<int8_t> dm(wr.Writer(), 2, 12);

        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);
    }

    void test_writerConsumer()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 1, 2, 5, 6, 9, 10};
        TestWriter<int8_t> srcWr(12);

        HIqDecimator<int8_t> dm(srcWr.Consumer(), 2, 12);
        TestWriter<int8_t> wr(dm.Consumer(), 12);

        ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);
    }

    void test_read_dont_collect()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 0, 0, 0, 0, 0, 0};
        TestReader<int8_t> rd(input, 12);

        HIqDecimator<int8_t> dm(rd.Reader(), 2, 12, false);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 6), 6);
        ASSERT_IS_EQUAL(rd.Reads, 1);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6), 0);
    }

    void test_write_dont_collect()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 0, 0, 0, 0, 0, 0};
        TestWriter<int8_t> wr(12);

        HIqDecimator<int8_t> dm(wr.Writer(), 2, 12, false);

        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 6);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);
    }

    void test_writerConsumer_dont_collect()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 0, 0, 0, 0, 0, 0};
        TestWriter<int8_t> srcWr(12);

        HIqDecimator<int8_t> dm(srcWr.Consumer(), 2, 12, false);
        TestWriter<int8_t> wr(dm.Consumer(), 6);

        ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);
    }

    void test_read_3()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 7, 8, 1, 2, 7, 8, 1, 2, 7, 8};
        TestReader<int8_t> rd(input, 12);

        HIqDecimator<int8_t> dm(rd.Reader(), 3, 12);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
        ASSERT_IS_EQUAL(rd.Reads, 3);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
    }

    void test_write_3()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 7, 8, 1, 2, 7, 8, 1, 2, 7, 8};
        TestWriter<int8_t> wr(12);

        HIqDecimator<int8_t> dm(wr.Writer(), 3, 12);

        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);
    }

    void test_read_5()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 11, 12, 9, 10, 7, 8, 5, 6, 3, 4};
        TestReader<int8_t> rd(input, 12);

        HIqDecimator<int8_t> dm(rd.Reader(), 5, 12);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
        ASSERT_IS_EQUAL(rd.Reads, 5);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
    }

    void test_write_5()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 11, 12, 9, 10, 7, 8, 5, 6, 3, 4};
        TestWriter<int8_t> wr(12);

        HIqDecimator<int8_t> dm(wr.Writer(), 5, 12);

        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12), 0);
    }

} hiqdecimator_test;