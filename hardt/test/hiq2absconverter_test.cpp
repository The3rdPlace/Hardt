#include <iostream>

#include "test.h"

class HIq2AbsConverter_Test: public Test
{
public:

    void run() override
    {
        UNITTEST(test_converter_as_writer);
        UNITTEST(test_converter_as_consumer);
        UNITTEST(test_converter_as_reader);
    }

    const char* name() override
    {
        return "HIq2AbsConverter";
    }

private:

    void test_converter_as_writer()
    {
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        int8_t expected[4] = {2, 4, 6, 8};

        TestWriter<int8_t > wr("hiq2absconverter_test_testwriter", 16);
        HIq2AbsConverter<int8_t> converter("hiq2absconverter_test_as_writer", wr.Writer(), 8);

        ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 4);
        for(int i = 0; i < 4; i++ ) {
            ASSERT_IS_EQUAL(wr.Received[i], expected[i]);
        }

        try
        {
            converter.Write(input, 6);
            ASSERT_FAIL("Expected HConverterIOException");
        }
        catch(HConverterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HConverterIOException, but got other type");
        }

        ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_converter_as_consumer()
    {
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        int8_t expected[4] = {2, 4, 6, 8};

        TestWriter<int8_t> srcWr("hiq2absconverter_test_testwriter_src", 8);
        HIq2AbsConverter<int8_t> converter("hiq2absconverter_test_as_consumer", srcWr.Consumer(), 8);
        TestWriter<int8_t > wr("hiq2absconverter_test_testwriter_wr", converter.Consumer(), 4);

        ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
        ASSERT_IS_EQUAL(srcWr.Samples, 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 4);
        for(int i = 0; i < 4; i++ ) {
            ASSERT_IS_EQUAL(wr.Received[i], expected[i]);
        }

        try
        {
            converter.Write(input, 6);
            ASSERT_FAIL("Expected HConverterIOException");
        }
        catch(HConverterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HConverterIOException, but got other type");
        }

        ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_converter_as_reader()
    {
        int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        int8_t expected[4] = {2, 4, 6, 8};

        TestReader<int8_t> rd("hiq2absconverter_test_testreader", output, 8);
        HIq2AbsConverter<int8_t> converter("hiq2absconverter_test_as_reader", &rd, 4);

        int8_t received[4];
        ASSERT_IS_EQUAL(converter.Read(received, 4), 4);
        ASSERT_IS_EQUAL(rd.Reads, 1);
        ASSERT_IS_EQUAL(rd.Samples, 8);
        for(int i = 0; i < 4; i++ ) {
            ASSERT_IS_EQUAL(received[i], expected[i]);
        }

        try
        {
            converter.Read(received, 6);
            ASSERT_FAIL("Expected HConverterIOException");
        }
        catch(HConverterIOException*)
        {}
        catch( ... )
        {
            ASSERT_FAIL("Expected HConverterIOException, but got other type");
        }

        ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(rd.Commands, 1);
    }

} hiq2absconverter_test;