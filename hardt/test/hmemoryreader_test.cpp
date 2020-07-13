#include "test.h"

#include <stdio.h>
#include <iostream>

#include "hmemoryreader.h"

class HMemoryReader_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_reads_int8);
            UNITTEST(test_reads_int32);
            UNITTEST(test_reads_infinite);
        }

        const char* name()
        {
            return "HMemoryReader";
        }

    private:

        void test_reads_int8()
        {
            int8_t input[20] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
            int8_t output[5] = { 0, 0, 0, 0, 0 };
            HMemoryReader<int8_t> rd(input, 20);
            int8_t expected[20] = { 1, 2, 3, 4, 5 };

            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 0);

            rd.Reset();
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
        }

        void test_reads_int32()
        {
            int32_t input[20] = { 100001, 100002, 100003, 100004, 100005, 100001, 100002, 100003, 100004, 100005, 100001, 100002, 100003, 100004, 100005, 100001, 100002, 100003, 100004, 100005 };
            int32_t output[5] = { 0, 0, 0, 0, 0 };
            HMemoryReader<int32_t> rd(input, 20);
            int32_t expected[20] = { 100001, 100002, 100003, 100004, 100005 };

            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 0);

            rd.Reset();
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
        }

        void test_reads_infinite()
        {
            int8_t input[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
            int8_t output[5] = { 0, 0, 0, 0, 0 };
            HMemoryReader<int8_t> rd(input, 20, true);
            int8_t expected[20] = { 1, 2, 3, 4, 5 };

            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);

            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL( memcmp((void*) output, (void*) expected, 5), 0);

            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
            ASSERT_IS_EQUAL(rd.Read(output, 5), 5);
        }

} hmemoryreader_test;