#include <iostream>

#include "test.h"

class HStreamProcessor_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_read_write);
            UNITTEST(test_command);
        }

        const char* name() override
        {
            return "HStreamProcessor";
        }

    private:

        void test_read_write()
        {
            bool terminated = false;
            int8_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};

            TestReader<int8_t> rdr("hstreamprocessor_test_testreader", expected, 14);
            TestWriter<int8_t> wr("hstreamprocessor_test_testwriter", 14);
            HStreamProcessor<int8_t> proc("hstreamprocessor_test_read_write", &wr, &rdr, 14, &terminated);

            proc.Run(1);

            ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int8_t)), 0);
        }

        void test_command()
        {
            bool terminated = false;

            TestReader<int8_t> rdr("hstreamprocessor_test_testreader", nullptr, 0);
            TestWriter<int8_t> wr("hstreamprocessor_test_testwriter", 0);
            HStreamProcessor<int8_t> proc("hstreamprocessor_test_command", &wr, &rdr, 0, &terminated);

            HCommandData data;
            data.State = true;
            ASSERT_IS_TRUE(proc.Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 0, data));
            ASSERT_IS_EQUAL(rdr.Commands, 1);
            ASSERT_IS_EQUAL(rdr.LastCommand.Class, (short) H_COMMAND_CLASS::NONE);
            ASSERT_IS_EQUAL(rdr.LastCommand.Opcode, (short) H_COMMAND_OPCODE::NOP);
            ASSERT_IS_EQUAL(rdr.LastCommand.Length, (short) 0);
            ASSERT_IS_TRUE(rdr.LastCommand.Data.State);
            ASSERT_IS_EQUAL(wr.Commands, 1);
            ASSERT_IS_EQUAL(wr.LastCommand.Class, (short) H_COMMAND_CLASS::NONE);
            ASSERT_IS_EQUAL(wr.LastCommand.Opcode, (short) H_COMMAND_OPCODE::NOP);
            ASSERT_IS_EQUAL(wr.LastCommand.Length, (short) 0);
            ASSERT_IS_TRUE(wr.LastCommand.Data.State);

            char content[] = "content";
            data.Content = (void*) content;
            ASSERT_IS_TRUE(proc.Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 7, data));
            ASSERT_IS_EQUAL(rdr.Commands, 2);
            ASSERT_IS_EQUAL(rdr.LastCommand.Class, (short) H_COMMAND_CLASS::NONE);
            ASSERT_IS_EQUAL(rdr.LastCommand.Opcode, (short) H_COMMAND_OPCODE::NOP);
            ASSERT_IS_EQUAL(rdr.LastCommand.Length, (short) 7);
            ASSERT_IS_EQUAL(wr.Commands, 2);
            ASSERT_IS_EQUAL(wr.LastCommand.Class, (short) H_COMMAND_CLASS::NONE);
            ASSERT_IS_EQUAL(wr.LastCommand.Opcode, (short) H_COMMAND_OPCODE::NOP);
            ASSERT_IS_EQUAL(wr.LastCommand.Length, (short) 7);
        }
} hstreamprocessor_test;
