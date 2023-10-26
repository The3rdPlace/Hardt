#include <iostream>

#include "test.h"

class HIqAddOrSubtractConverter_Test: public Test
{
    public:

        void run() override
        {
            TEST(test_converter_as_add_writer);
            TEST(test_converter_as_add_consumer);
            TEST(test_converter_as_add_reader);

            TEST(test_converter_as_subtract_writer);
            TEST(test_converter_as_subtract_consumer);
            TEST(test_converter_as_subtract_reader);
        }

        const char* name() override
        {
            return "HIqAddOrSubtractConverter";
        }

    private:

        void test_converter_as_add_writer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[4] = {3, 7, 11, 15};

            TestWriter<int8_t > wr("hiqaddorsubtractconverter_test_testwriter", 16);
            HIqAddOrSubtractConverter<int8_t> converter("hiqaddorsubtractconverter_test_as_add_writer", wr.Writer(), false, 8);

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

        void test_converter_as_add_consumer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[4] = {3, 7, 11, 15};

            TestWriter<int8_t> srcWr("hiqaddorsubtractconverter_test_testwriter_src", 8);
            HIqAddOrSubtractConverter<int8_t> converter("hiqaddorsubtractconverter_test_as_add_consumer", srcWr.Consumer(), false, 8);
            TestWriter<int8_t > wr("hiqaddorsubtractconverter_test_testwriter_wr", converter.Consumer(), 4);

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

        void test_converter_as_add_reader()
        {
            int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[4] = {3, 7, 11, 15};

            TestReader<int8_t> rd("hiqaddorsubtractconverter_test_testreader", output, 8);
            HIqAddOrSubtractConverter<int8_t> converter("hiqaddorsubtractconverter_test_as_add_reader", &rd, false, 4);

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

        void test_converter_as_subtract_writer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[4] = {-1, -1, -1, -1};

            TestWriter<int8_t > wr("hiqaddorsubtractconverter_test_testwriter", 16);
            HIqAddOrSubtractConverter<int8_t> converter("hiqaddorsubtractconverter_test_as_subtract_writer", wr.Writer(), true, 8);

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

        void test_converter_as_subtract_consumer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[4] = {-1, -1, -1, -1};

            TestWriter<int8_t> srcWr("hiqaddorsubtractconverter_test_testwriter_src", 8);
            HIqAddOrSubtractConverter<int8_t> converter("hiqaddorsubtractconverter_test_as_subtract_consumer", srcWr.Consumer(), true, 8);
            TestWriter<int8_t > wr("hiqaddorsubtractconverter_test_testwriter_wr", converter.Consumer(), 4);

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

        void test_converter_as_subtract_reader()
        {
            int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t expected[4] = {-1, -1, -1, -1};

            TestReader<int8_t> rd("hiqaddorsubtractconverter_test_testreader", output, 8);
            HIqAddOrSubtractConverter<int8_t> converter("hiqaddorsubtractconverter_test_as_subtract_reader", &rd, true, 4);

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

} hiqaddorsubtractconverter_test;