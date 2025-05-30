#include <iostream>

#include "test.h"

class HIirFilter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);
        }

        const char* name() override
        {
            return "HIirFilter";
        }

    private:

        /*
            ** We expect this to be correct, not the above **

            Length = 3
            Initial taps buffer: 0 0 0
            Initial output buffer: 0 0 0
            coefficients: b0=1, b1=2, b2=3, a0=-1, a1=-2

            First write:

                1 0 0
                0 0 0
                (1 * 1  +  0 * 2  +  0 * 3)  -  (0 * 1  +  0 * 2)  =  1

                2 1 0
                0 1 0
                (2 * 1  +  1 * 2  +  0 * 3)  -  (1 * 1  +  0 * 2)  =  3

                4 2 1
                0 3 1
                (4 * 1  +  2 * 2  +  1 * 3)  -  (3 * 1  +  1 * 2)  = 6

                8 4 2
                0 6 3
                (8 * 1  +  4 * 2  +  2 * 3)  -  (6 * 1  +  3 * 2) = 10

                16 8 4
                0 10 6
                (16 * 1  +  8 * 2  +  4 * 3)  -  (10 * 1  +  6 * 2) = 22

                32 16 8
                0 22 10
                (32 * 1  +  16 * 2  + 8 * 3)  -  (22 * 1  +  10 * 2) = 46

                Expected = 1 3 6 10 22 46

            Second write:

            Initial taps buffer: 32 16 8
            Initial output buffer: 46 22 10
            coefficients: b0=1, b1=2, b2=3, a0=-1, a1=-2

                1 32 16
                0 46 22
                (1 * 1  +  32  * 2  +  16 * 3)  -  (46 * 1  +  22 * 2) = 23

                2 1 32
                0 23 46
                (2 * 1  +  1 * 2  +  32 * 3)  -  (23 * 1  +  46 * 2) = -15

                Expected = 23 -15
        */

        // b and a coefficients are reversed in the list - most designers return the list
        // of coefficients this way!
        float coeefs[5] = { 1.0, 2.0, 3.0, -1.0, -2.0 };
        int16_t expected[6] = {1, 3, 6, 10, 22, 46};
        int16_t expectedNext[2] = {23, -15};

        void test_filter_as_writer()
        {
            TestWriter<int16_t> wr("hiirfilter_test_testwriter", 8);
            HIirFilter<int16_t> filter("hiirfilter_test_as_writer", wr.Writer(), coeefs, 5, 6);

            int16_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int16_t)), 0);

            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

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
            int16_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            TestReader<int16_t> rd("hiirfilter_test_testreader", output, 8);
            HIirFilter<int16_t> filter("hiirfilter_test_as_reader", &rd, coeefs, 5, 6);

            int16_t received[6];
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int16_t)), 0);

            ASSERT_IS_EQUAL(filter.Read(received, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

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
} HIirFilter_test;