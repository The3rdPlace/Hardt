#include <stdio.h>
#include <iostream>

#include "test.h"

class HReal2IqConverter_Test: public Test
{
public:

    void run()
    {
        UNITTEST(test_converter_as_writer);
        UNITTEST(test_converter_as_consumer);
        UNITTEST(test_converter_as_reader);
    }

    const char* name()
    {
        return "HReal2IqConverter";
    }

private:

    void test_converter_as_writer()
    {
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        TestWriter<std::complex<double>> wr(8);
        HReal2IqConverter<int8_t> converter(wr.Writer(), 100, 8);

        ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        for(int i = 0; i < 8; i++ ) {
            ASSERT_IS_EQUAL((int) round(std::abs(wr.Received[i])), (int) input[i]);
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

        TestWriter<int8_t> srcWr(8);
        HReal2IqConverter<int8_t> converter(srcWr.Consumer(), H_SAMPLE_RATE_8K, 8);
        TestWriter<std::complex<double>> wr(converter.Consumer(), 8);

        ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        for(int i = 0; i < 8; i++ ) {
            ASSERT_IS_EQUAL((int) round(std::abs(wr.Received[i])), (int) input[i]);
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

        TestReader<int8_t> rd(output, 8);
        HReal2IqConverter<int8_t> converter(&rd, H_SAMPLE_RATE_8K,8);

        std::complex<double> received[8];
        ASSERT_IS_EQUAL(converter.Read(received, 8), 8);
        ASSERT_IS_EQUAL(rd.Reads, 1);
        for(int i = 0; i < 8; i++ ) {
            ASSERT_IS_EQUAL((int) round(std::abs(received[i])), (int) output[i]);
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