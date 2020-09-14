#include <iostream>

#include "test.h"

class HIqMultiplier_Test: public Test
{
    public:

        void run()
        {
            TEST(test_multiplier_with_reader);
            UNITTEST(test_multiplier_with_writer);
        }

        const char* name()
        {
            return "HIqMultiplier";
        }

    public:

        void test_multiplier_with_reader()
        {
            int8_t output[10] = {0, 4, 1, 3, 2, 2, 3, 1, 4, 0};
            TestReader<int8_t> reader(output, 10);
            HLocalOscillator<int8_t> localOscillator(8000, 1000, 10);
            HIqMultiplier<int8_t> multiplier(&reader, 8000, 1000, 10, 10);

            int8_t received[10];
            int8_t oscillator[10];
            ASSERT_IS_EQUAL(multiplier.Read(received, 10), 10);
            ASSERT_IS_EQUAL(localOscillator.Read(oscillator, 10), 10);

            for(int i = 0; i < 10; i++)
            {
                std::cout << "[" << i << "] = " << std::to_string(received[i]) << std::endl;
                //ASSERT_IS_EQUAL(received[i], (int8_t) (i * oscillator[i]));
            }
            ASSERT_FAIL("STOP");
            
            ASSERT_IS_TRUE(multiplier.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_multiplier_with_writer()
        {
            TestWriter<int8_t> writer(5);
            HLocalOscillator<float> localOscillator(8000, 1000);
            HIqMultiplier<int8_t> multiplier(writer.Writer(), 8000, 1000, 10, 5);

            int8_t input[5] = {0, 1, 2, 3, 4};
            float oscillator[5];
            ASSERT_IS_EQUAL(multiplier.Write(input, 5), 5);
            ASSERT_IS_EQUAL(localOscillator.Read(oscillator, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], (int8_t) (input[i] * oscillator[i]));
            }

            ASSERT_IS_TRUE(multiplier.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

} hiqmultiplier_test;
