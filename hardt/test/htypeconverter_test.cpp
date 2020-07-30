#include <stdio.h>
#include <iostream>

#include "test.h"

class HTypeConverter_Test: public Test
{
    public:

        void run()
        {
            // Full test of one conversion
            UNITTEST(test_converter_int8_int16_as_writer);
            UNITTEST(test_converter_int8_int16_as_consumer);
            UNITTEST(test_converter_int8_int16_as_reader);

            // For other types, it suffices to test one method
            UNITTEST(test_converter_int8_int16_scaled);
            UNITTEST(test_converter_int8_int32);
            UNITTEST(test_converter_int8_int32_scaled);
            UNITTEST(test_converter_int16_int32);
            UNITTEST(test_converter_int16_int32_scaled);
            UNITTEST(test_converter_int8_float);
            UNITTEST(test_converter_int8_float_scaled);
            UNITTEST(test_converter_int8_double);
            UNITTEST(test_converter_int8_double_scaled);
            UNITTEST(test_converter_int16_float);
            UNITTEST(test_converter_int16_double);
            UNITTEST(test_converter_int16_double_scaled);
            UNITTEST(test_converter_int32_double);
            UNITTEST(test_converter_float_int16);
            UNITTEST(test_converter_double_int32);
            UNITTEST(test_converter_int32_int16);
            UNITTEST(test_converter_int32_int16_scaled);
            UNITTEST(test_converter_int32_int8);
            UNITTEST(test_converter_int32_int8_scaled);
            UNITTEST(test_converter_int16_int8);
            UNITTEST(test_converter_int16_int8_scaled);
        }

        const char* name()
        {
            return "HTypeConverter";
        }

    private:

        void test_converter_int8_int16_as_writer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int16_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int16_t > wr(8);
            HTypeConverter<int8_t, int16_t> converter(wr.Writer(), 8);

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

        void test_converter_int8_int16_as_consumer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int16_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<int8_t> srcWr(8);
            HTypeConverter<int8_t, int16_t> converter(srcWr.Consumer(), 8);
            TestWriter<int16_t > wr(converter.Consumer(), 8);

            ASSERT_IS_EQUAL(srcWr.Write(input, 8), 8);
            ASSERT_IS_EQUAL(srcWr.Samples, 8);
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

        void test_converter_int8_int16_as_reader()
        {
            int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int16_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestReader<int8_t> rd(output, 8);
            HTypeConverter<int8_t, int16_t> converter(&rd, 8);

            int16_t received[8];
            ASSERT_IS_EQUAL(converter.Read(received, 8), 8);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 8);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, sizeof(int16_t) * 8), 0);

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

        // Trivial tests below

        void test_converter_int8_int16_scaled()
        {
            int8_t input[8] =     {0, 2,   3,   4,    5,    6,    7,    127};
            int16_t expected[8] = {0, 516, 774, 1032, 1290, 1548, 1806, 32766};

            TestWriter<int16_t > wr(8);
            HTypeConverter<int8_t, int16_t> converter(wr.Writer(), 8, true);

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

        void test_converter_int8_int32()
        {
            int8_t input[8] =     {0, 2, 3, 4, 5, 6, 7,    127};
            int32_t expected[8] = {0, 2, 3, 4, 5, 6, 7,    127};

            TestWriter<int32_t > wr(8);
            HTypeConverter<int8_t, int32_t> converter(wr.Writer(), 8);

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

        void test_converter_int8_int32_scaled()
        {
            int8_t input[8] =     {0, 127,        0, 0 ,0 ,0 ,0 ,0};
            int32_t expected[8] = {0, 2147483640, 0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int32_t > wr(8);
            HTypeConverter<int8_t, int32_t> converter(wr.Writer(), 8, true);

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

        void test_converter_int16_int32()
        {
            int16_t input[8] =    {0, 2, 3, 4, 5, 6, 7,    32767};
            int32_t expected[8] = {0, 2, 3, 4, 5, 6, 7,    32767};

            TestWriter<int32_t > wr(8);
            HTypeConverter<int16_t, int32_t> converter(wr.Writer(), 8);

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

        void test_converter_int16_int32_scaled()
        {
            int16_t input[8] =    {0, 32767,      0, 0 ,0 ,0 ,0 ,0};
            int32_t expected[8] = {0, 2147450879, 0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int32_t > wr(8);
            HTypeConverter<int16_t, int32_t> converter(wr.Writer(), 8, true);

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

        void test_converter_int8_float()
        {
            int8_t input[8] =   {1, 2, 3, 4, 5, 6, 7, 8};
            float expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            TestWriter<float > wr(8);
            HTypeConverter<int8_t, float> converter(wr.Writer(), 8);

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

        void test_converter_int8_float_scaled()
        {
            int8_t input[8] =     {0, 2,   3,   4,    5,    6,    7,    127};
            float expected[8] = {0, 516, 774, 1032, 1290, 1548, 1806, 32766};

            TestWriter<float> wr(8);
            HTypeConverter<int8_t, float> converter(wr.Writer(), 8, true);

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

        void test_converter_int8_double()
        {
            int8_t input[8] =    {0, 2, 3, 4, 5, 6, 7,    127};
            double expected[8] = {0, 2, 3, 4, 5, 6, 7,    127};

            TestWriter<double > wr(8);
            HTypeConverter<int8_t, double> converter(wr.Writer(), 8);

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

        void test_converter_int8_double_scaled()
        {
            int8_t input[8] =    {0, 127,        0, 0 ,0 ,0 ,0 ,0};
            double expected[8] = {0, 2147483640, 0, 0 ,0 ,0 ,0 ,0};

            TestWriter<double > wr(8);
            HTypeConverter<int8_t, double> converter(wr.Writer(), 8, true);

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

        void test_converter_int16_float()
        {
            int16_t input[8] =    {0, 2, 3, 4, 5, 6, 7,    32767};
            float expected[8] = {0, 2, 3, 4, 5, 6, 7,    32767};

            TestWriter<float> wr(8);
            HTypeConverter<int16_t, float> converter(wr.Writer(), 8);

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

        void test_converter_int16_double()
        {
            int16_t input[8] =    {0, 2, 3, 4, 5, 6, 7,    32767};
            double expected[8] = {0, 2, 3, 4, 5, 6, 7,    32767};

            TestWriter<double > wr(8);
            HTypeConverter<int16_t, double> converter(wr.Writer(), 8);

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

        void test_converter_int16_double_scaled()
        {
            int16_t input[8] =    {0, 32767,      0, 0 ,0 ,0 ,0 ,0};
            double expected[8] = {0, 2147450879, 0, 0 ,0 ,0 ,0 ,0};

            TestWriter<double> wr(8);
            HTypeConverter<int16_t, double> converter(wr.Writer(), 8, true);

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

        void test_converter_int32_double()
        {
            int32_t input[8] =    {0, 2, 3, 4, 5, 6, 7, 2147483647};
            double expected[8] =  {0, 2, 3, 4, 5, 6, 7, 2147483647};

            TestWriter<double > wr(8);
            HTypeConverter<int32_t, double> converter(wr.Writer(), 8);

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

        void test_converter_float_int16()
        {
            float input[8] =      {0, 2, 3, 4, 5, 6, 7,    32767};
            int16_t expected[8] = {0, 2, 3, 4, 5, 6, 7,    32767};

            TestWriter<int16_t > wr(8);
            HTypeConverter<float, int16_t> converter(wr.Writer(), 8);

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

        void test_converter_double_int32()
        {
            double input[8] =    {0, 2, 3, 4, 5, 6, 7, 2147483647};
            int32_t expected[8] =  {0, 2, 3, 4, 5, 6, 7, 2147483647};

            TestWriter<int32_t > wr(8);
            HTypeConverter<double, int32_t> converter(wr.Writer(), 8);

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

        void test_converter_int32_int16()
        {
            int32_t input[8] =    {0, 2147450879, 0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int16_t > wr(8);
            HTypeConverter<int32_t, int16_t> converter(wr.Writer(), 8);

            try
            {
                converter.Write(input, 8);
                ASSERT_FAIL("Expected HConverterIOException");
            }
            catch(HConverterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HConverterIOException, but got other type");
            }
        }

        void test_converter_int32_int16_scaled()
        {
            int32_t input[8] =    {0, 2147450879, 0, 0 ,0 ,0 ,0 ,0};
            int16_t expected[8] = {0, 32767,      0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int16_t > wr(8);
            HTypeConverter<int32_t, int16_t> converter(wr.Writer(), 8, true);

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

        void test_converter_int32_int8()
        {
            int32_t input[8] =    {0, 2147450879, 0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int8_t > wr(8);
            HTypeConverter<int32_t, int8_t> converter(wr.Writer(), 8);

            try
            {
                converter.Write(input, 8);
                ASSERT_FAIL("Expected HConverterIOException");
            }
            catch(HConverterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HConverterIOException, but got other type");
            }
        }

        void test_converter_int32_int8_scaled()
        {
            int32_t input[8] =   {0, 2147450879, 0, 0 ,0 ,0 ,0 ,0};
            int8_t expected[8] = {0, 126,        0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int8_t > wr(8);
            HTypeConverter<int32_t, int8_t> converter(wr.Writer(), 8, true);
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

        void test_converter_int16_int8()
        {
            int16_t input[8] = {0, 516, 774, 1032, 1290, 1548, 1806, 32766};

            TestWriter<int8_t > wr(8);
            HTypeConverter<int16_t, int8_t> converter(wr.Writer(), 8);

            try
            {
                converter.Write(input, 8);
                ASSERT_FAIL("Expected HConverterIOException");
            }
            catch(HConverterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HConverterIOException, but got other type");
            }
        }

        void test_converter_int16_int8_scaled()
        {
            int16_t input[8] =   {0, 516, 774, 1032, 1290, 1548, 1806, 32766};
            int8_t expected[8] = {0, 2,   3,   4,    5,    6,    7,    127};

            TestWriter<int8_t > wr(8);
            HTypeConverter<int16_t, int8_t> converter(wr.Writer(), 8, true);

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

} htypeconverter_test;