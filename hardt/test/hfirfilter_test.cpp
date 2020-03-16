#include <stdio.h>
#include <iostream>

#include "test.h"

class HFirFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);
        }

        const char* name()
        {
            return "HFirFilter";
        }

    private:

        /*
            First write:

                Initial taps buffer: 0 0 0

                1 0 0 = 1 0 0 = 1
                2 1 0 = 2 2 0 = 4
                4 2 1 = 4 4 3 = 11
                8 4 2 = 8 8 6 = 22
                16 8 4 = 16 16 12 = 44
                32 16 8 = 32 32 24 = 88


                Expected = 1 4 11 22 44 88

            Second write:

                Initial taps buffer: 32 16 8

                1 32 16 = 1 64 48 = 113
                2 1 32 = 2 2 96 = 100

                Expected = 113 100
         */
        float coeefs[3] = { 1.0, 2.0, 3.0 };
        int8_t expected[6] = {1, 4, 11, 22, 44, 88};
        int8_t expectedNext[2] = {113, 100};

        void test_filter_as_writer()
        {
            TestWriter<int8_t> wr(8);
            HFirFilter<int8_t> filter(wr.Writer(), coeefs, 3, 6);

            int8_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 2 * sizeof(int8_t)), 0);

            try
            {
                filter.Write(input, 8);
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
            int8_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            TestReader<int8_t> rd(output, 8);
            HFirFilter<int8_t> filter(&rd, coeefs, 3, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Read(received, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 2 * sizeof(int8_t)), 0);

            try
            {
                filter.Read(received, 8);
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
} hfirfilter_test;