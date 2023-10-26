#include <iostream>

#include "test.h"

class HHumFilter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_hum_filter_as_writer);
            UNITTEST(test_hum_filter_as_reader);
        }

        const char* name() override
        {
            return "HHumFilter";
        }

    public:

        int8_t input[1024];

        HHumFilter_Test()
        {
            for( int i = 0; i < 1024; i++ )
            {
                input[i] = 0;
            }
            input[0] = 1;
        }

        void test_hum_filter_as_writer()
        {
            TestWriter<int8_t> wr("hhumfilter_test_testwriter", 1024);
            HHumFilter<int8_t> filter("hhumfilter_test_as_writer", wr.Writer(), 48000, 50, 200, 1024);

            ASSERT_IS_EQUAL(filter.Write(input, 1024), 1024);
            for( int i = 0; i < 1024; i++)
            {
                ASSERT_IS_EQUAL((int) wr.Received[i], 0);
            }

            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);

            try
            {
                filter.Write(input, 1025);
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

        void test_hum_filter_as_reader()
        {
            TestReader<int8_t> rd("hhumfilter_test_testreader", input, 1024);
            HHumFilter<int8_t> filter("hhumfilter_test_as_reader", &rd, 48000, 50, 200, 1024);

            int8_t received[1024];
            ASSERT_IS_EQUAL(filter.Read(received, 1024), 1024);
            for( int i = 0; i < 1024; i++)
            {
                ASSERT_IS_EQUAL((int) received[i], 0);
            }

            ASSERT_IS_EQUAL(filter.Read(received, 2), 2);

            try
            {
                filter.Read(received, 1025);
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
} HHumfilter_test;