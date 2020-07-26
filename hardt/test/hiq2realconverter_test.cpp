#include <stdio.h>
#include <iostream>

#include "test.h"

class HIq2RealConverter_Test: public Test
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
        return "HIq2RealConverter";
    }

private:

    void test_converter_as_writer()
    {
        std::complex<double> input[] = {
            {0, 1},
            {1.90211, 0.618034},
            {1.76336, -2.42705},
            {-2.35114, -3.23607},
            {-4.75528, 1.54508},
            {0, 6},
            {6.6574, 2.16312},
            {4.70228, -6.47214}
        };
        int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        TestWriter<int8_t> wr(8);
        HIq2RealConverter<int8_t> converter(wr.Writer(), 100, 8);

        ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);
        for( int i = 0; i < 8; i++ ) {
            std::cout << "[" << i << "] = " << std::to_string(wr.Received[i]) << " - " << std::to_string(expected[i]) << std::endl;
            if( i == 0 ) {
                bool result = wr.Received[i] == 0 || wr.Received[i] == expected[i] || wr.Received[i] == expected[i + 1];
                ASSERT_IS_TRUE(result);
            } else if( i>= 7 ) {
                bool result = wr.Received[i] == expected[i - 1] || wr.Received[i] == expected[i] || wr.Received[i] == 9;
                ASSERT_IS_TRUE(result);
            } else {
                bool result = wr.Received[i] == expected[i - 1] || wr.Received[i] == expected[i] || wr.Received[i] == expected[i + 1];
                ASSERT_IS_TRUE(result);
            }
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
        std::complex<double> input[] = {
            {0, 1},
            {1.90211, 0.618034},
            {1.76336, -2.42705},
            {-2.35114, -3.23607},
            {-4.75528, 1.54508},
            {0, 6},
            {6.6574, 2.16312},
            {4.70228, -6.47214}
        };
        int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        TestWriter<std::complex<double>> srcWr(8);
        HIq2RealConverter<int8_t> converter(srcWr.Consumer(), H_SAMPLE_RATE_8K, 8);
        TestWriter<int8_t> wr(converter.Consumer(), 8);

        ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
        ASSERT_IS_EQUAL(wr.Writes, 1);

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
        std::complex<double> output[] = {
            {0, 1},
            {1.90211, 0.618034},
            {1.76336, -2.42705},
            {-2.35114, -3.23607},
            {-4.75528, 1.54508},
            {0, 6},
            {6.6574, 2.16312},
            {4.70228, -6.47214}
        };
        int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        TestReader<std::complex<double>> rd(output, 8);
        HIq2RealConverter<int8_t> converter(&rd, H_SAMPLE_RATE_8K,8);

        int8_t received[8];
        ASSERT_IS_EQUAL(converter.Read(received, 8), 8);
        ASSERT_IS_EQUAL(rd.Reads, 1);


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

    void foo() {
    }

} hiq2realconverter_test;