#include <iostream>

#include "test.h"

class HCollector_Test: public Test
{
public:

    void run() override
    {
        TEST(test_collector_as_writer);
        TEST(test_collector_as_reader);
        TEST(test_collector_as_consumer);
    }

    const char* name() override
    {
        return "HCollector";
    }

private:

    void test_collector_as_writer()
    {
        int8_t input[6] = {2, 4, 6, 8, 10, 12};
        int8_t expected[24] = {2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12};
        TestWriter<int8_t> wr("hcollector_test_testwriter", 24);
        HCollector<int8_t> collector("hcollector_test_as_writer", wr.Writer(), 6, 24);

        ASSERT_IS_EQUAL(collector.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(collector.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(collector.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(collector.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 24);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 24 * sizeof(int8_t)), 0);

        try
        {
            collector.Write(input, 8);
            ASSERT_FAIL("Expected HWriterIOException");
        }
        catch(HWriterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HWriterIOException, but got other type");
        }

        ASSERT_IS_TRUE(collector.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_collector_as_reader()
    {
        int8_t output[6] = {2, 4, 6, 8, 10, 12};
        int8_t expected[24] = {2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12};
        TestReader<int8_t> rd("hcollector_test_testreader", output, 24);
        HCollector<int8_t> collector("hcollector_test_as_reader", &rd, 6, 24);

        int8_t received[24];
        ASSERT_IS_EQUAL(collector.Read(received, 24), 24);
        ASSERT_IS_EQUAL(rd.Reads, 4);
        ASSERT_IS_EQUAL(rd.Samples, 24);
        ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 24 * sizeof(int8_t)), 0);

        try
        {
            collector.Read(received, 8);
            ASSERT_FAIL("Expected HReaderIOException");
        }
        catch(HReaderIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HReaderIOException, but got other type");
        }

        ASSERT_IS_TRUE(collector.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }

    void test_collector_as_consumer()
    {
        int8_t input[6] = {2, 4, 6, 8, 10, 12};
        int8_t expected[24] = {2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12, 2, 4, 6, 8, 10, 12};
        TestWriter<int8_t> srcWr("hcollector_test_testwriter_src", 6);
        HCollector<int8_t> collector("hcollector_test_as_consumer", srcWr.Consumer(), 6, 24);
        TestWriter<int8_t> wr("hcollector_test_testwriter_wr", collector.Consumer(), 24);

        ASSERT_IS_EQUAL(srcWr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(srcWr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(srcWr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 0);
        ASSERT_IS_EQUAL(srcWr.Write(input, 6), 6);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 24);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 24 * sizeof(int8_t)), 0);

        try
        {
            collector.Write(input, 8);
            ASSERT_FAIL("Expected HWriterIOException");
        }
        catch(HWriterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HWriterIOException, but got other type");
        }

        ASSERT_IS_TRUE(collector.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

} hcollector_test;