#include <stdio.h>
#include <iostream>

#include "test.h"

class HGaussianFilter_Test: public Test
{
public:

    void run()
    {
        // Full test to check the output of the HGaussianFilter component
        UNITTEST(test_filter_as_writer);

        // Simple tests, just to check that read and write behaves as expected
        UNITTEST(test_filter_as_consumer);
        UNITTEST(test_filter_as_reader);
    }

    const char* name()
    {
        return "HGaussianFilter";
    }

private:

    void test_filter_as_writer()
    {
        // Input signal
        int8_t input[8]    = { 10, 112, 35, 93, 56, 60, 73, 8 };

        // Expected
        int8_t expected[6] = { 41, 41, 0, 41, 41, 0};

        TestWriter<int8_t> wr(6);
        HGaussianFilter<int8_t> flt(wr.Writer(), 6, 2, 6);

        ASSERT_IS_EQUAL(flt.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);

        DUMP(wr.Received, 6);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int8_t) * 6), 0);

        try
        {
            flt.Write(input, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(flt.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_filter_as_consumer()
    {
        // Input signal
        int8_t input[8]    = { 1, 2, 3, 4, 5, 6, 7, 8 };

        TestWriter<int8_t> srcWr(8);
        HGaussianFilter<int8_t> flt(srcWr.Consumer(), 6, 12, 6);
        TestWriter<int8_t> wr(flt.Consumer(),8);

        ASSERT_IS_EQUAL(srcWr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);

        try
        {
            srcWr.Write(input, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(srcWr.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_filter_as_reader()
    {
        // Input signal
        int8_t input[8]    = { 1, 2, 3, 4, 5, 6, 7, 8 };

        TestReader<int8_t> rd(input, 8);
        HGaussianFilter<int8_t> flt(rd.Reader(), 6, 18, 6);

        int8_t received[6];
        ASSERT_IS_EQUAL(flt.Read(received, 6), 6);
        ASSERT_IS_EQUAL(rd.Reads, 1);

        try
        {
            flt.Read(received, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(flt.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }
} hgaussianfilter_test;