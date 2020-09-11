#include <iostream>

#include "test.h"

class HIqTranslateByFour_Test: public Test
{
public:

    void run()
    {
        // Full test to check the output of the HTranslateByFour component
        UNITTEST(test_translate_as_I_writer);
        UNITTEST(test_translate_as_Q_writer);

        // Simple tests, just to check that read and write behaves as expected
        UNITTEST(test_translate_as_consumer);
        UNITTEST(test_translate_as_reader);
    }

    const char* name()
    {
        return "HIqTranslateByFour";
    }

private:

    void test_translate_as_I_writer()
    {
        // Input signal
        int8_t input[12] = {1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1};
        int8_t expected[12] = {1, 2, 0, 0, -5, -6, 0, 0, 4, 3, 0, 0};

        TestWriter<int8_t> wr(6);
        HIqTranslateByFour<int8_t> tr(wr.Writer(), 6);

        ASSERT_IS_EQUAL(tr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int8_t) * 6), 0);

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

    void test_translate_as_Q_writer()
    {
        // Input signal
        int8_t input[12] = {1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1};
        int8_t expected[12] = {0, 0, -3, -4, 0, 0, 6, 5 , 0, 0, -2, -1};

        TestWriter<int8_t> wr(12);
        HIqTranslateByFour<int8_t> tr(wr.Writer(), 12, true);

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
        TestWriter<int8_t> srcWr(8);
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        HIqTranslateByFour<int8_t> tr(srcWr.Consumer(), 6);
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
        HIqTranslateByFour<int8_t> tr(&rd, 6);

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
} hiqtranslatebyfour_test;