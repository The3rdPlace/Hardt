#include "test.h"

class HBufferedWriter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(few_blocks);
            UNITTEST(more_blocks);
            UNITTEST(more_blocks_no_buffering);
            UNITTEST(many_blocks);
            UNITTEST(inactive_drain);
            UNITTEST(active_drain);
            UNITTEST(many_writes);
        }

        const char* name() override
        {
            return "HBufferedWriter";
        }

    private:

        void few_blocks()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_few_blocks", writer.Writer(), 8, 10);

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
            ASSERT_IS_EQUAL(writer.Writes, 0);
        }

        void more_blocks()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_more_blocks", writer.Writer(), 8, 10);


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
            ASSERT_IS_EQUAL(writer.Writes, 0);
        }

        void more_blocks_no_buffering()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_more_blocks_no_buffering", writer.Writer(), 8, 10, false);

            ASSERT_IS_EQUAL(bufferedWriter.Enabled(), false);

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

            ASSERT_IS_EQUAL(bufferedWriter.Reserved(), 10);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 0);
            ASSERT_IS_EQUAL(writer.Writes, 12);
        }

        void many_blocks()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_many_blocks", writer.Writer(), 8, 10);

            for( int i = 0; i < 100000; i++ )
            {
                bufferedWriter.Write(input, 8);
            }

            ASSERT_IS_EQUAL(bufferedWriter.Reserved(), 100000);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 100000);
            ASSERT_IS_EQUAL(writer.Writes, 0);
        }

        void inactive_drain()
        {
            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_inactive_drain", writer.Writer(), 8, 10);

            bufferedWriter.Start();

            sleep(3);

            bufferedWriter.Stop();
        }

        void active_drain()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_active_drain", writer.Writer(), 8, 10);

            bufferedWriter.Start();

            sleep(1);
            for( int i = 0; i < 20; i++ ) {
                bufferedWriter.Write(input, 8);
                usleep(200);
            }
            sleep(1);

            bufferedWriter.Stop();

            ASSERT_IS_EQUAL(writer.Writes, 20);
            ASSERT_IS_EQUAL(bufferedWriter.Reserved(), 10);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 0);
        }

        void many_writes()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> writer("hbufferedwriter_test_testwriter", 8, true, 10);
            HBufferedWriter<int8_t> bufferedWriter("hbufferedwriter_test_many_writes", writer.Writer(), 8, 10);

            bufferedWriter.Start();

            for( int i = 0; i < 20000; i++ ) {
                bufferedWriter.Write(input, 8);
            }

            ASSERT_IS_GREATER_THAN(bufferedWriter.Used(), 0);
            bufferedWriter.Flush();
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 0);

            bufferedWriter.Stop();

            ASSERT_IS_EQUAL(writer.Writes, 20000);
            ASSERT_IS_EQUAL(bufferedWriter.Used(), 0);
        }

} hbufferedwriter_test;