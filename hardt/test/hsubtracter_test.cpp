#include <iostream>

#include "test.h"

class HSubtracter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_subtracter_with_readers);
            UNITTEST(test_subtracter_with_reader_and_writer);
            UNITTEST(test_subtracter_with_reader_and_consumer);
            UNITTEST(test_subtracter_with_multiplexed_writers);
            UNITTEST(test_subtracter_with_multiplexed_writers_and_consumer);
        }

        const char* name() override
        {
            return "HSubtracter";
        }

    public:

        void test_subtracter_with_readers()
        {
            int8_t input[5] = {0, 1, 2, 3, 4};
            TestReader<int8_t> reader_1("hsubtracter_test_testreader_1", input, 5);
            TestReader<int8_t> reader_2("hsubtracter_test_testreader_2", input, 5);
            HSubtracter<int8_t> subtracter("hsubtracter_test_with_readers", &reader_1, &reader_2, 5);

            int8_t output[5];
            ASSERT_IS_EQUAL(subtracter.Read(output, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(output[i], (int8_t) (i - i));
            }

            ASSERT_IS_TRUE(subtracter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader_1.Commands, 1);
            ASSERT_IS_EQUAL(reader_2.Commands, 1);
        }

        void test_subtracter_with_reader_and_writer()
        {
            int8_t input_1[5] = {0, 1, 2, 3, 4};
            TestReader<int8_t> reader("hsubtracter_test_testreader", input_1, 5);
            TestWriter<int8_t> writer("hsubtracter_test_testwriter", 5);
            HSubtracter<int8_t> subtracter("hsubtracter_test_with_reader_and_writer", reader.Reader(), writer.Writer(), 5);

            int8_t input_2[5] = {4, 3, 2, 1, 0};
            int8_t expected[5] = {-4, -2, 0, 2, 4};
            ASSERT_IS_EQUAL(subtracter.Write(input_2, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(subtracter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

        void test_subtracter_with_reader_and_consumer()
        {
            int8_t input_1[5] = {0, 1, 2, 3, 4};
            TestReader<int8_t> reader("hsubtracter_test_testreader", input_1, 5);
            TestWriter<int8_t> writer_1("hsubtracter_test_testwriter_1", 5);
            HSubtracter<int8_t> subtracter("hsubtracter_test_with_reader_and_consumer", reader.Reader(), writer_1.Consumer(), 5);
            TestWriter<int8_t> writer_2("hsubtracter_test_testwriter_2", subtracter.Consumer(), 5);

            int8_t input_2[5] = {4, 3, 2, 1, 0};
            int8_t expected[5] = {-4, -2, 0, 2, 4};
            ASSERT_IS_EQUAL(writer_1.Write(input_2, 5), 5);

            ASSERT_IS_EQUAL(writer_2.Writes, 1);
            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer_2.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(writer_1.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
            ASSERT_IS_EQUAL(writer_2.Commands, 1);
        }

        void test_subtracter_with_multiplexed_writers()
        {
            TestWriter<int8_t> writer("hsubtracter_test_testwriter", 5);
            HSubtracter<int8_t> subtracter("hsubtracter_test_with_multiplexed_writers", writer.Writer(), 5);

            int8_t input_1[5] = {0, 1, 2, 3, 4};
            int8_t input_2[5] = {4, 3, 2, 1, 0};
            int8_t expected[5] = {-4, -2, 0, 2, 4};
            ASSERT_IS_EQUAL(subtracter.Write(input_1, 5), 5);
            ASSERT_IS_EQUAL(subtracter.Write(input_2, 5), 5);

            ASSERT_IS_EQUAL(writer.Writes, 1);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(subtracter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

        void test_subtracter_with_multiplexed_writers_and_consumer()
        {
            TestWriter<int8_t> writer_1("hsubtracter_test_testwriter_1", 5);
            HSubtracter<int8_t> subtracter("hsubtracter_test_with_multiplexed_writers_and_consumers", writer_1.Consumer(), 5);
            TestWriter<int8_t> writer_2("hsubtracter_test_testwriter_2", subtracter.Consumer(), 5);

            int8_t input_1[5] = {0, 1, 2, 3, 4};
            int8_t input_2[5] = {4, 3, 2, 1, 0};
            int8_t expected[5] = {-4, -2, 0, 2, 4};
            ASSERT_IS_EQUAL(writer_1.Write(input_1, 5), 5);
            ASSERT_IS_EQUAL(writer_1.Write(input_2, 5), 5);

            ASSERT_IS_EQUAL(writer_2.Writes, 1);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer_2.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(writer_1.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer_2.Commands, 1);
        }

} hsubtracter_test;
