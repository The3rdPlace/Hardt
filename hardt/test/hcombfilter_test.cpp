#include <stdio.h>
#include <iostream>

#include "test.h"

class HCombFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_feedforward_filter_as_writer);
            UNITTEST(test_feedforward_filter_as_reader);
        }

        const char* name()
        {
            return "HCombFilter";
        }

    public:

        int8_t input[1024];

        HCombFilter_Test()
        {
            for( int i = 0; i < 1024; i++ )
            {
                input[i] = 0;
            }
            input[0] = 1;
        }

        void test_feedforward_filter_as_writer()
        {
            TestWriter<int8_t> wr(1024);
            HCombFilter<int8_t> filter(wr.Writer(), 48000, 50, -1, 1024);

            ASSERT_IS_EQUAL(filter.Write(input, 1024), 1024);
            ASSERT_IS_EQUAL((int) wr.Received[0], 1);
            ASSERT_IS_EQUAL((int) wr.Received[960], -1);
            for( int i = 0; i < 1024; i++)
            {
                if( i != 0 && i != 960 )
                {
                    ASSERT_IS_EQUAL((int) wr.Received[i], 0);
                }
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

        void test_feedforward_filter_as_reader()
        {
            TestReader<int8_t> rd(input, 1024);
            HCombFilter<int8_t> filter(&rd, 48000, 50, -1, 1024);

            int8_t received[1024];
            ASSERT_IS_EQUAL(filter.Read(received, 1024), 1024);
            ASSERT_IS_EQUAL((int) received[0], 1);
            ASSERT_IS_EQUAL((int) received[960], -1);
            for( int i = 0; i < 1024; i++)
            {
                if( i != 0 && i != 960 )
                {
                    ASSERT_IS_EQUAL((int) received[i], 0);
                }
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
} hcombfilter_test;