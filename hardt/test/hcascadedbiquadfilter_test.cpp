#include <iostream>

#include "test.h"

class HCascadedBiQuadFilter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_single_filters_as_writer);
            UNITTEST(test_single_filters_as_consumer);
            UNITTEST(test_cascaded_filter_as_writer);
            UNITTEST(test_cascaded_filter_as_consumer);
            UNITTEST(test_single_filters_as_reader);
            UNITTEST(test_cascaded_filter_as_reader);
        }

        const char* name() override
        {
            return "HCascadedBiQuadFilter";
        }

    private:

        /*
            The cascaded biquad filter stacks two (in this case identical) iir filters.
            The output is thus expected to be the same as when writing through two
            individual biquads with the same coefficients.

            Length = 3
            Initial taps buffer: 0 0 0
            Initial output buffer: 0 0 0
            coefficients: b0=1, b1=2, b2=3, a0=-1, a1=-2

            First write:

                First BiQuad:
                
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

                Second BiQuad:

                    1 0 0
                    0 0 0
                    (1 * 1  +  0 * 2  +  0 * 3)  -  (0 * 1  +  0 * 2)  =  1

                    3 1 0
                    0 1 0
                    (3 * 1  +  1 * 2  +  0 * 3)  -  (1 * 1  +  0 * 2)  =  4

                    6 3 1
                    0 4 1
                    (6 * 1  +  3 * 2  +  1 * 3)  -  (4 * 1  +  1 * 2)  = 9

                    10 6 3
                    0 9 4
                    (10 * 1  +  6 * 2  +  3 * 3)  -  (9 * 1  +  4 * 2) = 14

                    22 10 6
                    0 14 9
                    (22 * 1  +  10 * 2  +  6 * 3)  -  (14 * 1  +  9 * 2) = 28

                    46 22 10
                    0 28 14
                    (46 * 1  +  22 * 2  + 10 * 3)  -  (28 * 1  +  14 * 2) = 64

                    Expected = 1 4 9 14 28 64

            Second write:

                First BiQuad:

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

                Second BiQuad:

                    Initial taps buffer: 46 22 10
                    Initial output buffer: 64 28 14
                    coefficients: b0=1, b1=2, b2=3, a0=-1, a1=-2

                        23 46 22
                        0 64 28
                        (23 * 1  +  46  * 2  +  22 * 3)  -  (64 * 1  +  28 * 2) = 61

                        -15 23 46
                        0 61 64
                        (-15 * 1  +  23 * 2  +  46 * 3)  -  (61 * 1  +  64 * 2) = -20

                        Expected = 61 -20
        */

        // b and a coefficients are reversed in the list - most designers return the list
        // of coefficients this way!
        float coeffs[10] = { 1.0, 2.0, 3.0, -1.0, -2.0, 1.0, 2.0, 3.0, -1.0, -2.0 };
        int16_t expected[6] = {1, 4, 9, 14, 28, 64};
        int16_t expectedNext[2] = {61, -20};

        void test_single_filters_as_writer()
        {
            TestWriter<int16_t> wr("hcascadedbiquadfilter_test_testwriter", 8);
            HIirFilter<int16_t> filter2("hcascadedbiquadfilter_test_filter_2", wr.Writer(), coeffs, 5, 6);
            HIirFilter<int16_t> filter("hcascadedbiquadfilter_test_filter", filter2.Writer(), &coeffs[5], 5, 6);

            int16_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int16_t)), 0);
            
            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);
        }

        void test_single_filters_as_consumer()
        {
            HNullWriter<int16_t> dummy("hcascadedbiquadfilter_test_hnullwriter"); // Can not write to next writer, only needed for constructor args
            HIirFilter<int16_t> filter("hcascadedbiquadfilter_test_filter", dummy.Consumer(), &coeffs[5], 5, 6);
            HIirFilter<int16_t> filter2("hcascadedbiquadfilter_test_filter_2", filter.Consumer(), coeffs, 5, 6);
            TestWriter<int16_t> wr("hcascadedbiquadfilter_test_testwriter", filter2.Consumer(), 8);
            
            int16_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int16_t)), 0);
            
            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);
        }

        void test_cascaded_filter_as_writer()
        {
            TestWriter<int16_t> wr("hcascadedbiquadfilter_test_testwriter", 8);
            HCascadedBiQuadFilter<int16_t>* filter = new HCascadedBiQuadFilter<int16_t>("hcascadedbiquadfilter_test_as_writer", wr.Writer(), coeffs, 10, 6);

            int16_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter->Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int16_t)), 0);
            
            ASSERT_IS_EQUAL(filter->Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

            try
            {
                filter->Write(input, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(filter->Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);

            delete filter;
        }

        void test_cascaded_filter_as_consumer()
        {
            HNullWriter<int16_t> dummy("hcascadedbiquadfilter_test_nullwriter"); // Can not write to next writer, only needed for constructor args
            HCascadedBiQuadFilter<int16_t>* filter = new HCascadedBiQuadFilter<int16_t>("hcascadedbiquadfilter_test_as_consumer", dummy.Consumer(), coeffs, 10, 6);
            TestWriter<int16_t> wr("hcascadedbiquadfilter_test_testwriter", filter->Consumer(), 8);

            int16_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter->Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int16_t)), 0);
            
            ASSERT_IS_EQUAL(filter->Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

            try
            {
                filter->Write(input, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(filter->Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_single_filters_as_reader()
        {
            int16_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            TestReader<int16_t> rd("hcascadedbiquadfilter_test_testreader", output, 8);
            HIirFilter<int16_t> filter("hcascadedbiquadfilter_test_filter", &rd, coeffs, 5, 6);
            HIirFilter<int16_t> filter2("hcascadedbiquadfilter_test_filter_2", filter.Reader(), &coeffs[5], 5, 6);

            int16_t received[6];
            ASSERT_IS_EQUAL(filter2.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int16_t)), 0);

            ASSERT_IS_EQUAL(filter2.Read(received, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);
        }

        void test_cascaded_filter_as_reader()
        {
            int16_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            TestReader<int16_t> rd("hcascadedbiquadfilter_test_testreader", output, 8);
            HCascadedBiQuadFilter<int16_t>* filter = new HCascadedBiQuadFilter<int16_t>("hcascadedbiquadfilter_test_as_reader", rd.Reader(), coeffs, 10, 6);

            int16_t received[6];
            ASSERT_IS_EQUAL(filter->Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int16_t)), 0);

            ASSERT_IS_EQUAL(filter->Read(received, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

            try
            {
                filter->Read(received, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(filter->Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);

            delete filter;
        }

} HCascadedBiQuadFilter_test;