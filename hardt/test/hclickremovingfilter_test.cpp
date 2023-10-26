#include <stdio.h>
#include <iostream>

#include "test.h"

class HClickRemovingFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_clickremovingfilter_as_writer);
            UNITTEST(test_clickremovingfilter_as_reader);
        }

        const char* name()
        {
            return "HClickRemovingFilter";
        }

    private:

        int8_t expected[8] = {2, 4, 3, 4, 0, 6, 4, 2};

        void test_clickremovingfilter_as_writer()
        {
            TestWriter<int8_t> wr("hclickremovingfilter_test_testwriter", 8);
            int8_t input[8] = {2, 4, 3, 4, 10, 6, 4, 2};
            HClickRemovingFilter<int8_t> crf("hclickremovingfilter_test_as_writer", wr.Writer(), 8);

            ASSERT_IS_EQUAL(crf.Write(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            try
            {
                crf.Write(input, 9);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(crf.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_clickremovingfilter_as_reader()
        {
            int8_t output[8] = {2, 4, 3, 4, 10, 6, 4, 2};
            TestReader<int8_t> rd("hclickremovingfilter_test_testreader", output, 8);
            HClickRemovingFilter<int8_t> crf("hclickremovingfilter_test_as_reader", &rd, 8);

            int8_t received[8];
            ASSERT_IS_EQUAL(crf.Read(received, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 8 * sizeof(int8_t)), 0);

            try
            {
                crf.Read(received, 9);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(crf.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }
} hclickremovingfilter_test;