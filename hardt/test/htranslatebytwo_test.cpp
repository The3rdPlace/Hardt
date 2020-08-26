#include <stdio.h>
#include <iostream>

#include "test.h"

class HTranslateByTwo_Test: public Test
{
public:

    void run()
    {
        // Full test to check the output of the HTranslateByTwo component
        TEST(test_translate_as_writer);

        // Simple tests, just to check that read and write behaves as expected
        TEST(test_translate_as_consumer);
        TEST(test_translate_as_reader);
    }

    const char* name()
    {
        return "HTranslateByTwo";
    }

private:

    void test_translate_as_writer()
    {
        // Input signal
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        int8_t expected[6] = {1, -2, 3, -4, 5, -6};

        TestWriter<int8_t> wr(6);
        HTranslateByTwo<int8_t> tr(wr.Writer(), 6);

        ASSERT_IS_EQUAL(tr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int8_t) * 6), 0);

        try
        {
            tr.Write(input, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(tr.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_translate_as_consumer()
    {
        TestWriter<int8_t> srcWr(8);
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        HTranslateByTwo<int8_t> tr(srcWr.Consumer(), 6);
        TestWriter<int8_t> wr(tr.Consumer(),8);

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

    void test_translate_as_reader()
    {
        int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        TestReader<int8_t> rd(output, 8);
        HTranslateByTwo<int8_t> tr(&rd, 6);

        int8_t received[6];
        ASSERT_IS_EQUAL(tr.Read(received, 6), 6);
        ASSERT_IS_EQUAL(rd.Reads, 1);

        try
        {
            tr.Read(received, 8);
            ASSERT_FAIL("Expected HFilterIOException");
        }
        catch(HFilterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HFilterIOException, but got other type");
        }

        ASSERT_IS_TRUE(tr.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }
} htranslatebytwo_test;