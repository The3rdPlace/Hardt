#include <stdio.h>
#include <iostream>

#include "test.h"

class HComplexConverter_Test: public Test
{
public:

    void run()
    {
        // Full test of one conversion
        UNITTEST(test_converter_int8_complex_as_writer);
        UNITTEST(test_converter_int8_complex_as_consumer);
        UNITTEST(test_converter_int8_int16_as_reader);

        // Trivial tests
        UNITTEST(test_converter_int16_complex_as_writer);
        UNITTEST(test_converter_int32_complex_as_writer);
        UNITTEST(test_converter_float_complex_as_writer);
        UNITTEST(test_converter_double_complex_as_writer);
        UNITTEST(test_converter_complex_int8_as_writer);
        UNITTEST(test_converter_complex_int16_as_writer);
        UNITTEST(test_converter_complex_int32_as_writer);
        UNITTEST(test_converter_complex_float_as_writer);
        UNITTEST(test_converter_complex_double_as_writer);
    }

    const char* name()
    {
        return "HComplexConverter";
    }

    private:

        void test_converter_int8_complex_as_writer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<int8_t> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestWriter<std::complex<int8_t>> wr(8);
            HComplexConverter<int8_t, std::complex<int8_t>> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(std::complex<int8_t>) * 8), 0);

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

        void test_converter_int8_complex_as_consumer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<int8_t> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestWriter<int8_t> srcWr(8);
            HComplexConverter<int8_t, std::complex<int8_t>> converter(srcWr.Consumer(), 8);
            TestWriter<std::complex<int8_t>> wr(converter.Consumer(), 8);

            ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
            ASSERT_IS_EQUAL(srcWr.Samples, 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(std::complex<int8_t>) * 8), 0);

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

        void test_converter_int8_int16_as_reader()
        {
            int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<int8_t> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestReader<int8_t> rd(output, 8);
            HComplexConverter<int8_t, std::complex<int8_t>> converter(&rd, 8);

            std::complex<int8_t> received[8];
            ASSERT_IS_EQUAL(converter.Read(received, 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, sizeof(std::complex<int8_t>) * 8), 0);

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

        void test_converter_int16_complex_as_writer()
        {
            int16_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<int16_t> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestWriter<std::complex<int16_t>> wr(8);
            HComplexConverter<int16_t, std::complex<int16_t>> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(std::complex<int16_t>) * 8), 0);

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

        void test_converter_int32_complex_as_writer()
        {
            int32_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<int32_t> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestWriter<std::complex<int32_t>> wr(8);
            HComplexConverter<int32_t, std::complex<int32_t>> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(std::complex<int32_t>) * 8), 0);

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

        void test_converter_float_complex_as_writer()
        {
            float input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<float> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestWriter<std::complex<float>> wr(8);
            HComplexConverter<float, std::complex<float>> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(std::complex<float>) * 8), 0);

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

        void test_converter_double_complex_as_writer()
        {
            double input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            std::complex<double> expected[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};

            TestWriter<std::complex<double>> wr(8);
            HComplexConverter<double, std::complex<double>> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(std::complex<double>) * 8), 0);

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

        void test_converter_complex_int8_as_writer()
        {
            std::complex<int8_t> input[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};
            int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> wr(8);
            HComplexConverter<std::complex<int8_t>, int8_t> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int8_t) * 8), 0);

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

        void test_converter_complex_int16_as_writer()
        {
            std::complex<int16_t> input[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};
            int16_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int16_t> wr(8);
            HComplexConverter<std::complex<int16_t>, int16_t> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int16_t) * 8), 0);

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

        void test_converter_complex_int32_as_writer()
        {
            std::complex<int32_t> input[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};
            int32_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int32_t> wr(8);
            HComplexConverter<std::complex<int32_t>, int32_t> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(int32_t) * 8), 0);

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

        void test_converter_complex_float_as_writer()
        {
            std::complex<float> input[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};
            float expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<float> wr(8);
            HComplexConverter<std::complex<float>, float> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(float) * 8), 0);

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

        void test_converter_complex_double_as_writer()
        {
            std::complex<double> input[8] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};
            double expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<double> wr(8);
            HComplexConverter<std::complex<double>, double> converter(wr.Writer(), 8);

            ASSERT_IS_EQUAL(converter.Write(input, 8), 8);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, sizeof(double) * 8), 0);

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

} hcomplexconverter_test;