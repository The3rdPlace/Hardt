#include <stdio.h>
#include <iostream>

#include "test.h"

class HStreamProcessor_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_read_write);
        }

        const char* name()
        {
            return "HStreamProcessor";
        }

    private:

        void test_read_write()
        {
            bool terminated = false;
            int8_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};

            TestReader<int8_t> rdr(expected, 14);
            TestWriter<int8_t> wr(14);
            HStreamProcessor<int8_t> proc(&wr, &rdr, 14, &terminated);

            proc.Run(1);

            ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int8_t)), 0);
        }
} hstreamprocessor_test;
