#include <stdio.h>
#include <iostream>

#include "test.h"

class HComplex2MultiplexedConverter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_converter_read);
            UNITTEST(test_converter_write);
        }

        const char* name()
        {
            return "HComplex2MultiplexedConverter";
        }
   
        void test_converter_read()
        {
            std::complex<int8_t> input[3] = {{1, 2}, {3, 4}, {5, 6}};
            int8_t expected[6] = {1, 2, 3, 4, 5, 6};
            int8_t output[6] = {0, 0 ,0, 0, 0, 0};

            TestReader<std::complex<int8_t>> rd(input, 3);
            HComplex2MultiplexedConverter<int8_t> converter(&rd, 3);

            ASSERT_IS_EQUAL(converter.Read(output, 6), 6);
            ASSERT_IS_EQUAL(rd.Reads, 1);
            ASSERT_IS_EQUAL(rd.Samples, 3);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_converter_write()
        {
            std::complex<int8_t> input[3] = {{1, 2}, {3, 4}, {5, 6}};
            int8_t expected[6] = {1, 2, 3, 4, 5, 6};

            TestWriter<int8_t> wr(6);
            HComplex2MultiplexedConverter<int8_t> converter(&wr, 3);

            ASSERT_IS_EQUAL(converter.Write(input, 3), 3);
            ASSERT_IS_EQUAL(wr.Writes, 1);
            ASSERT_IS_EQUAL(wr.Samples, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(wr.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

} hcomplex2multiplexed_test;
