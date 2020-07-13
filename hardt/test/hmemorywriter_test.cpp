#include "test.h"

#include <stdio.h>
#include <iostream>

#include "hmemorywriter.h"

class HMemoryWriter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_writes_int8);
            UNITTEST(test_writes_int32);
            UNITTEST(test_writes_infinite);
        }

        const char* name()
        {
            return "HMemoryWriter";
        }

    private:

        void test_writes_int8()
        {
            int8_t output[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            HMemoryWriter<int8_t> wr(output, 20);
            int8_t input[5] = { 1, 2, 3, 4, 5 };
            int8_t expected[20] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };

            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 0);

            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 20), 0);

            wr.Reset();
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
        }

        void test_writes_int32()
        {
            int32_t output[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            HMemoryWriter<int32_t> wr(output, 20);
            int32_t input[5] = { 100001, 100002, 100003, 100004, 100005 };
            int32_t expected[20] = { 100001, 100002, 100003, 100004, 100005, 100001, 100002, 100003, 100004, 100005, 100001, 100002, 100003, 100004, 100005, 100001, 100002, 100003, 100004, 100005 };

            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 0);

            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 20 * sizeof(int32_t)), 0);

            wr.Reset();
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
        }

        void test_writes_infinite()
        {
            int8_t output[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            HMemoryWriter<int8_t> wr(output, 20, true);
            int8_t input[5] = { 1, 2, 3, 4, 5 };
            int8_t expected[20] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 20), 0);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 20), 0);

            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 20), 0);

            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
            ASSERT_IS_EQUAL(wr.Write(input, 5), 5);
        }

} hmemorywriter_test;