#include <iostream>

#include "test.h"

class HPassThrough_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_passthrough_as_writer);
            UNITTEST(test_passthrough_as_reader);
        }

        const char* name() override
        {
            return "HPassThrough";
        }

    private:

        void test_passthrough_as_writer()
        {
            int8_t input[6] = {2, 4, 6, 8, 10, 12};
            TestWriter<int8_t> wr("hpassthrough_test_testwriter", 6);
            HPassThrough<int8_t> passthrough("hpassthrough_test_as_writer", wr.Writer(), 6);

            ASSERT_IS_EQUAL(passthrough.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) input, 6 * sizeof(int8_t)), 0);

            try
            {
                passthrough.Write(input, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(passthrough.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_passthrough_as_reader()
        {
            int8_t output[6] = {2, 4, 6, 8, 10, 12};
            TestReader<int8_t> rd("hpassthrough_test_testreader", output, 6);
            HPassThrough<int8_t> passthrough("hpassthrough_test_as_reader", &rd, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(passthrough.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) output, 6 * sizeof(int8_t)), 0);

            try
            {
                passthrough.Read(received, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(passthrough.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }
} hpassthrough_test;