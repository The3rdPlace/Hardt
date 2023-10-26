#include <iostream>

#include "test.h"

class HMultiplexed2ComplexConverter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_converter_read);
            UNITTEST(test_converter_write);
        }

        const char* name() override
        {
            return "HMultiplexed2ComplexConverter";
        }
   
        void test_converter_read()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            std::complex<int8_t> expected[3] = {{1, 2}, {3, 4}, {5, 6}};
            std::complex<int8_t> output[3] = {0, 0 ,0};

            TestReader<int8_t> rd("hmultiplexed2complexconverter_testreader", input, 6);
            HMultiplexed2ComplexConverter<int8_t> converter("hmultiplexed2complexconverter_read", &rd, 6);

            ASSERT_IS_EQUAL(converter.Read(output, 3), 3);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 6);

            for(int i = 0; i < 3; i++)
            {
                ASSERT_IS_EQUAL(output[i].real(), expected[i].real());
                ASSERT_IS_EQUAL(output[i].imag(), expected[i].imag());
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_converter_write()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            std::complex<int8_t> expected[3] = {{1, 2}, {3, 4}, {5, 6}};

            TestWriter<std::complex<int8_t>> wr("hmultiplexed2complexconverter_testwriter", 3);
            HMultiplexed2ComplexConverter<int8_t> converter("hmultiplexed2complexconverter_write", &wr, 6);

            ASSERT_IS_EQUAL(converter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 3);

            for(int i = 0; i < 3; i++)
            {
                ASSERT_IS_EQUAL(wr.Received[i].real(), expected[i].real());
                ASSERT_IS_EQUAL(wr.Received[i].imag(), expected[i].imag());
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

} hmultiplexed2complexconverter_test;
