#include <iostream>

#include "test.h"

class HIqMultiplier_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_multiplier_with_reader);
            UNITTEST(test_multiplier_with_writer);
        }

        const char* name() override
        {
            return "HIqMultiplier";
        }

    public:

        void test_multiplier_with_reader()
        {
            int8_t output[10] = {0, 4, 1, 3, 2, 2, 3, 1, 4, 0};
            TestReader<int8_t> reader("hiqmultiplier_test_testreader", output, 10);
            HLocalOscillator<int8_t> sinOscillator("hiqmultiplier_test_sine_generator", 8000, 1000, 10);
            HLocalOscillator<int8_t> cosOscillator("hiqmultiplier_test_cos_generator", 8000, 1000, 10, 0);
            HIqMultiplier<int8_t> multiplier("hiqmultiplier_test_with_reader", &reader, 8000, 1000, 10, 10);

            int8_t received[10];
            int8_t sin[5];
            int8_t cos[5];
            ASSERT_IS_EQUAL(multiplier.Read(received, 10), 10);
            ASSERT_IS_EQUAL(sinOscillator.Read(sin, 5), 5);
            ASSERT_IS_EQUAL(cosOscillator.Read(cos, 5), 5);

            for(int i = 0; i < 10; i += 2)
            {
                int8_t ac = output[i] * sin[i / 2];
                int8_t bd = output[i + 1] * cos[i / 2];
                int8_t ab_cd = (output[i] + output[i + 1]) * (sin[i / 2] + cos[i / 2]);

                int8_t R = (ac - bd) / 10;
                int8_t J = (ab_cd - ac - bd) / 10;

                ASSERT_IS_EQUAL(R, received[i]);
                ASSERT_IS_EQUAL(J, received[i + 1]);
            }

            ASSERT_IS_TRUE(multiplier.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_multiplier_with_writer()
        {
            TestWriter<int8_t> writer("hiqmultiplier_test_testwriter", 10);
            HLocalOscillator<int8_t> sinOscillator("hiqmultiplier_test_sine_generator", 8000, 1000, 10);
            HLocalOscillator<int8_t> cosOscillator("hiqmultiplier_test_cos_generator", 8000, 1000, 10, 0);
            HIqMultiplier<int8_t> multiplier("hiqmultiplier_test_with_writer", writer.Writer(), 8000, 1000, 10, 10);

            int8_t input[10] = {0, 4, 1, 3, 2, 2, 3, 1, 4, 0};
            int8_t sin[5];
            int8_t cos[5];
            ASSERT_IS_EQUAL(multiplier.Write(input, 10), 10);
            ASSERT_IS_EQUAL(sinOscillator.Read(sin, 5), 5);
            ASSERT_IS_EQUAL(cosOscillator.Read(cos, 5), 5);

            for(int i = 0; i < 10; i += 2)
            {
                int8_t ac = input[i] * sin[i / 2];
                int8_t bd = input[i + 1] * cos[i / 2];
                int8_t ab_cd = (input[i] + input[i + 1]) * (sin[i / 2] + cos[i / 2]);

                int8_t R = (ac - bd) / 10;
                int8_t J = (ab_cd - ac - bd) / 10;

                ASSERT_IS_EQUAL(R, writer.Received[i]);
                ASSERT_IS_EQUAL(J, writer.Received[i + 1]);
            }

            ASSERT_IS_TRUE(multiplier.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

} hiqmultiplier_test;
