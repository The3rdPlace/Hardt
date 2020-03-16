#include <stdio.h>
#include <iostream>

#include "test.h"

class HMux_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_mux);
        }

        const char* name()
        {
            return "HDeMux";
        }

    private:


        void test_mux()
        {
            int8_t stream_1[4] = {1, 3, 5, 7};
            int8_t stream_2[4] = {2, 4, 6, 8};
            TestReader<int8_t> rd_1(stream_1, 4);
            TestReader<int8_t> rd_2(stream_2, 4);
            std::vector< HReader<int8_t>* > readers = { &rd_1, &rd_2 };
            HMux<int8_t> mux(readers, 8);

            int8_t input[8];
            int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            ASSERT_IS_EQUAL(mux.Read(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) input, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mux.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd_1.Commands, 1);
            ASSERT_IS_EQUAL(rd_2.Commands, 1);
        }
} hmux_test;