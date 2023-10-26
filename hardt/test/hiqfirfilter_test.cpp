#include <iostream>

#include "test.h"

class HIqFirFilter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);
        }

        const char* name() override
        {
            return "HIqFirFilter";
        }

    private:

        void test_filter_as_writer()
        {
            TestWriter<int8_t> wr("hiqfirfilter_test_testwriter", 8);
            float coeefs[3] = { 1.0, 2.0, 3.0 };
            HIqFirFilter<int8_t> filter("hiqfirfilter_test_as_writer", wr.Writer(), coeefs, 3, 12);

            // I channel = {1, 2, 4, 8, 16, 32, 0, 0};
            // Q channel = {32, 16, 8, 4, 2, 1, 0, 0};
            int8_t input[16] = {1, 32, 2 , 16, 4 , 8 ,8 ,4 ,16, 2, 32, 1 , 0, 0, 0, 0};

            // I channel = {1, 4, 11, 22, 44, 88}
            // Q channel = {32, 80, -120, 68, 34, 17}
            int8_t expected[12] = {1, 32, 4, 80, 11, -120, 22, 68, 44, 34, 88, 17};

            // I channel = {113, 100}
            // Q channel = {40, 83}
            int8_t expectedNext[4] = {113, 40, 100, 83};

            ASSERT_IS_EQUAL(filter.Write(input, 12), 12);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 12 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Write(input, 4), 4);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 4 * sizeof(int8_t)), 0);

            try
            {
                filter.Write(input, 16);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(filter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_filter_as_reader()
        {
            // I channel = {1, 2, 4, 8, 16, 32, 0, 0};
            // Q channel = {32, 16, 8, 4, 2, 1, 0, 0};
            int8_t input[16] = {1, 32, 2 , 16, 4 , 8 ,8 ,4 ,16, 2, 32, 1 , 0, 0, 0, 0};

            // I channel = {1, 4, 11, 22, 44, 88}
            // Q channel = {32, 80, -120, 68, 34, 17}
            int8_t expected[12] = {1, 32, 4, 80, 11, -120, 22, 68, 44, 34, 88, 17};

            // I channel = {113, 100}
            // Q channel = {40, 83}
            int8_t expectedNext[4] = {113, 40, 100, 83};

            TestReader<int8_t> rd("hiqfirfilter_test_testreader", input, 16);
            float coeefs[3] = { 1.0, 2.0, 3.0 };
            HIqFirFilter<int8_t> filter("hiqfirfilter_test_as_reader", &rd, coeefs, 3, 12);

            int8_t received[12];
            ASSERT_IS_EQUAL(filter.Read(received, 12), 12);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 12 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Read(received, 4), 4);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 4 * sizeof(int8_t)), 0);

            try
            {
                filter.Read(received, 16);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(filter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }
} hiqfirfilter_test;