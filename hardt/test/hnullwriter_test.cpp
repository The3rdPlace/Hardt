#include "test.h"

class HNullWriter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_writes_int8);
            UNITTEST(test_writes_int32);
        }

        const char* name()
        {
            return "HNullWriter";
        }

    private:

        void test_writes_int8()
        {
            HNullWriter<int8_t> wr("hnullwriter_test_int8");
            int8_t input[1024];

            ASSERT_IS_EQUAL(wr.Write(input, 10), 10);
            ASSERT_IS_EQUAL(wr.Write(input, 512), 512);
            ASSERT_IS_EQUAL(wr.Write(input, 1024), 1024);

            ASSERT_IS_EQUAL(wr.Write(input, 2048), 2048);
        }

        void test_writes_int32()
        {
            HNullWriter<int32_t> wr("hnullwriter_test_int32");
            int32_t input[1024];

            ASSERT_IS_EQUAL(wr.Write(input, 10), 10);
            ASSERT_IS_EQUAL(wr.Write(input, 512), 512);
            ASSERT_IS_EQUAL(wr.Write(input, 1024), 1024);

            ASSERT_IS_EQUAL(wr.Write(input, 2048), 2048);
        }

} hnullwriter_test;