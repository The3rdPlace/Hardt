#include <iostream>

#include "test.h"

class HReal2IqConverter_Test: public Test
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
        return "HReal2IqConverter";
    }

private:

    void test_converter_as_writer()
    {
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        TestWriter<int8_t > wr("hreal2iqconverter_test_testwriter", 16);
        HReal2IqConverter<int8_t> converter("hreal2iqconverter_test_as_writer", wr.Writer(), 8);

        ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 16);
        int j = 0;
        for(int i = 0; i < 16; i += 2 ) {
            ASSERT_IS_EQUAL(wr.Received[i], input[j++]);
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

        TestWriter<int8_t> srcWr("hreal2iqconverter_test_testwriter", 8);
        HReal2IqConverter<int8_t> converter("hreal2iqconverter_test_as_consumer", srcWr.Consumer(), 8);
        TestWriter<int8_t > wr("hreal2iqconverter_test_testwriter", converter.Consumer(), 16);

        ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
        ASSERT_IS_EQUAL(srcWr.Samples, 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        ASSERT_IS_EQUAL(wr.Samples, 16);
        int j = 0;
        for(int i = 0; i < 16; i += 2 ) {
            ASSERT_IS_EQUAL(wr.Received[i], input[j++]);
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

        TestReader<int8_t> rd("hreal2iqconverter_test_testreader", output, 8);
        HReal2IqConverter<int8_t> converter("hreal2iqconverter_test_as_reader", &rd, 16);

        int8_t received[16];
        ASSERT_IS_EQUAL(converter.Read(received, 16), 16);
        ASSERT_IS_EQUAL(rd.Reads, 1);
        ASSERT_IS_EQUAL(rd.Samples, 8);
        int j = 0;
        for(int i = 0; i < 16; i += 2 ) {
            ASSERT_IS_EQUAL(received[i], output[j++]);
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

} hreal2iqconverter_test;