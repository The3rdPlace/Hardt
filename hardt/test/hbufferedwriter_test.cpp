#include "test.h"

class HBufferedWriter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(few_blocks);
            UNITTEST(more_blocks);
            UNITTEST(many_blocks);
        }

        const char* name()
        {
            return "HBufferedWriter";
        }

    private:

        class BlockedBufferedWriter: public HBufferedWriter<int8_t>
        {
            public:

                BlockedBufferedWriter(int reserved):
                    HBufferedWriter<int8_t>(reserved)
                {}
        };

        void few_blocks()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            BlockedBufferedWriter bufferedWriter(10);
            TestWriter<int8_t> writer(bufferedWriter.Writer(), 8);

            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);

            ASSERT_IS_EQUAL(bufferedWriter.Reserved(), 10);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 10);
        }

        void more_blocks()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            BlockedBufferedWriter bufferedWriter(10);
            TestWriter<int8_t> writer(bufferedWriter.Writer(), 8);

            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);
            bufferedWriter.Write(input, 8);

            ASSERT_IS_EQUAL(bufferedWriter.Reserved(), 20);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 12);
        }

        void many_blocks()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            BlockedBufferedWriter bufferedWriter(10);
            TestWriter<int8_t> writer(bufferedWriter.Writer(), 8);

            for( int i = 0; i < 100000; i++ )
            {
                bufferedWriter.Write(input, 8);
            }

            ASSERT_IS_EQUAL(bufferedWriter.Reserved(), 100000);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 100000);
        }


} hbufferedwriter_test;