#include <iostream>

#include "test.h"

class HIqDecimator_Test: public Test
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

        UNITTEST(test_write_continuous);
    }

    const char* name() override
    {
        return "HIqDecimator";
    }

private:

    void test_read()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 1, 2, 5, 6, 9, 10};
        TestReader<int8_t> rd("hiqdecimator_test_testreader", input, 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_read", rd.Reader(), 2, 12);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
        ASSERT_IS_EQUAL(rd.Reads, 2);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
    }

    void test_write()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 1, 2, 5, 6, 9, 10};
        TestWriter<int8_t> wr("hiqdecimator_test_testwriter", 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_write", wr.Writer(), 2, 12);

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
        TestWriter<int8_t> srcWr("hiqdecimator_test_testwriter_src", 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_writerconsumer", srcWr.Consumer(), 2, 12);
        TestWriter<int8_t> wr("hiqdecimator_test_testwriter_wr", dm.Consumer(), 12);

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
        TestReader<int8_t> rd("hiqdecimator_test_testreader", input, 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_read_dont_collect", rd.Reader(), 2, 12, false);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 6), 6);
        ASSERT_IS_EQUAL(rd.Reads, 1);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6), 0);
    }

    void test_write_dont_collect()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 0, 0, 0, 0, 0, 0};
        TestWriter<int8_t> wr("hiqdecimator_test_testwriter", 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_write_dont_collect", wr.Writer(), 2, 12, false);

        ASSERT_IS_EQUAL(dm.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 6);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);
    }

    void test_writerConsumer_dont_collect()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 5, 6, 9, 10, 0, 0, 0, 0, 0, 0};
        TestWriter<int8_t> srcWr("hiqdecimator_test_testwriter_src", 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_writerconsumer_dont_collect", srcWr.Consumer(), 2, 12, false);
        TestWriter<int8_t> wr("hiqdecimator_test_testwriter_wr", dm.Consumer(), 6);

        ASSERT_IS_EQUAL(srcWr.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);
    }

    void test_read_3()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 7, 8, 1, 2, 7, 8, 1, 2, 7, 8};
        TestReader<int8_t> rd("hiqdecimator_test_testreader", input, 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_read_3", rd.Reader(), 3, 12);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
        ASSERT_IS_EQUAL(rd.Reads, 3);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
    }

    void test_write_3()
    {
        int8_t input[48] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                          25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
                          37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48};
        int8_t expected[] = {1, 2, 7, 8, 1, 2, 7, 8, 1, 2, 7, 8};
        TestWriter<int8_t> wr("hiqdecimator_test_testwriter", 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_write_3", wr.Writer(), 3, 12);

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
        TestReader<int8_t> rd("hiqdecimator_test_testreader", input, 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_read_5", rd.Reader(), 5, 12);

        int8_t received[12];

        ASSERT_IS_EQUAL(dm.Read(received, 12), 12);
        ASSERT_IS_EQUAL(rd.Reads, 5);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12), 0);
    }

    void test_write_5()
    {
        int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int8_t expected[] = {1, 2, 11, 12, 9, 10, 7, 8, 5, 6, 3, 4};
        TestWriter<int8_t> wr("hiqdecimator_test_testwriter", 12);

        HIqDecimator<int8_t> dm("hiqdecimator_test_write_5", wr.Writer(), 5, 12);

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

    void test_write_continuous()
    {
        int8_t input[120];
        int8_t expected_1[12];
        int8_t expected_2[12];

        for( int i = 0; i < 60; i += 2 ) {
            input[i] = i;
            input[i + 1] = i + 1;
            if( i % 5 == 0 ) {
                expected_1[i / 5] = i;
                expected_1[(i / 5) + 1] = i + 1;
            }
        }

        for( int i = 60; i < 120; i += 2 ) {
            input[i] = i;
            input[i + 1] = i + 1;
            if( i % 5 == 0 ) {
                expected_2[(i / 5) - 12] = i;
                expected_2[(i / 5) - 12 + 1] = i + 1;
            }
        }

        TestWriter<int8_t> wr("hiqdecimator_test_testwriter", 12);
        HIqDecimator<int8_t> dm("hiqdecimator_test_write_continuous", wr.Writer(), 5, 12);

        int i = 0;
        while( wr.Writes == 0 ) {
            ASSERT_IS_EQUAL(dm.Write(&input[i], 12), 12);
            i += 12;
        }
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_1, 12), 0);

        while( wr.Writes == 1 ) {
            ASSERT_IS_EQUAL(dm.Write(&input[i], 12), 12);
            i += 12;
        }
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected_2, 12), 0);
    }

} hiqdecimator_test;