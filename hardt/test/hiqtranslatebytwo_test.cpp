#include <iostream>

#include "test.h"

class HIqTranslateByTwo_Test: public Test
{
public:

    void run() override
    {
        // Full test to check the output of the HTranslateByTwo component
        UNITTEST(test_translate_as_writer);

        // Simple tests, just to check that read and write behaves as expected
        UNITTEST(test_translate_as_consumer);
        UNITTEST(test_translate_as_reader);
    }

    const char* name() override
    {
        return "HIqTranslateByTwo";
    }

private:

    void test_translate_as_writer()
    {
        // Input signal
        int8_t input[12] = {1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1};
        int8_t expected[12] = {1, 2, -3, -4, 5, 6, -6, -5, 4, 3, -2, -1};

        TestWriter<int8_t> wr("hiqtranslatebytwo_test_testwriter", 12);
        HIqTranslateByTwo<int8_t> tr("hiqtranslatebytwo_test_as_writer", wr.Writer(), 12);

        ASSERT_IS_EQUAL(tr.Write(input, 12), 12);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int8_t) * 12), 0);

        try
        {
            tr.Write(input, 16);
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
        TestWriter<int8_t> srcWr("hiqtranslatebytwo_test_testwriter_src", 8);
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        HIqTranslateByTwo<int8_t> tr("hiqtranslatebytwo_test_as_consumer", srcWr.Consumer(), 6);
        TestWriter<int8_t> wr("hiqtranslatebytwo_test_testwriter", tr.Consumer(),8);

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
        TestReader<int8_t> rd("hiqtranslatebytwo_test_testreader", output, 8);
        HIqTranslateByTwo<int8_t> tr("hiqtranslatebytwo_test_as_reader", &rd, 6);

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
} hiqtranslatebytwo_test;