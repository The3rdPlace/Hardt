#include <stdio.h>
#include <iostream>

#include "test.h"

class HDeMux_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_demux_write);
            UNITTEST(test_demux_read);
        }

        const char* name()
        {
            return "HDeMux";
        }

    private:

        void test_demux_write()
        {
            TestWriter<int8_t> wr_1(8);
            TestWriter<int8_t> wr_2(8);
            std::vector< HWriter<int8_t>* > writers = { &wr_1, &wr_2 };
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected_1[8] = {1, 3, 5, 7, 0, 0, 0, 0};
            int8_t expected_2[8] = {2, 4, 6, 8, 0, 0, 0, 0};
            HDeMux<int8_t> demux(writers, 8);

            ASSERT_IS_EQUAL(demux.Write(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr_1.Received, (void*) expected_1, 8 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr_2.Received, (void*) expected_2, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(demux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr_1.Commands, 1);
            ASSERT_IS_EQUAL(wr_2.Commands, 1);
        }

        void test_demux_read()
        {
            int8_t input[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
            int8_t expected_1[8] = {1, 3, 5, 7, 9, 11, 13, 15};
            int8_t expected_2[8] = {2, 4, 6, 8, 10, 12, 14, 16};
            TestReader<int8_t> rd(input, 16);
            HDeMux<int8_t> demux(&rd, 2, 8);

            int8_t received[8];
            ASSERT_IS_EQUAL(demux.Read(received, 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 16);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_1, 8 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(demux.Read(received, 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 16);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_2, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(demux.Read(received, 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 2);
            ASSERT_IS_EQUAL(rd.Samples, 32);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_1, 8 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(demux.Read(received, 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 2);
            ASSERT_IS_EQUAL(rd.Samples, 32);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_2, 8 * sizeof(int8_t)), 0);
        }

} hdemux_test;