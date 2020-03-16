#include <stdio.h>
#include <iostream>

#include "test.h"

class HDeMux_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_demux);
        }

        const char* name()
        {
            return "HDeMux";
        }

    private:

        int8_t expected_1[8] = {1, 3, 5, 7, 0, 0, 0, 0};
        int8_t expected_2[8] = {2, 4, 6, 8, 0, 0, 0, 0};

        void test_demux()
        {
            TestWriter<int8_t> wr_1(8);
            TestWriter<int8_t> wr_2(8);
            std::vector< HWriter<int8_t>* > writers = { &wr_1, &wr_2 };
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            HDeMux<int8_t> demux(writers, 8);

            ASSERT_IS_EQUAL(demux.Write(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr_1.Received, (void*) expected_1, 8 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr_2.Received, (void*) expected_2, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(demux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr_1.Commands, 1);
            ASSERT_IS_EQUAL(wr_2.Commands, 1);
        }
} hdemux_test;