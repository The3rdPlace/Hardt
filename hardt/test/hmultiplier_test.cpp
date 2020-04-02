#include <stdio.h>
#include <iostream>

#include "test.h"

class HMultiplier_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_multiplier_with_readers);
            UNITTEST(test_multiplier_with_reader_and_writer);
        }

        const char* name()
        {
            return "HMultiplier";
        }

    public:

        void test_multiplier_with_readers()
        {
            int8_t output[5] = {0, 1, 2, 3, 4};
            TestReader<int8_t> reader(output, 5);
            HLocalOscillator<int8_t> localOscillator(8000, 1000, 10);
            HMultiplier<int8_t> multiplier(&reader, 8000, 1000, 10, 5);

            int8_t received[5];
            int8_t oscillator[5];
            ASSERT_IS_EQUAL(multiplier.Read(received, 5), 5);
            ASSERT_IS_EQUAL(localOscillator.Read(oscillator, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(received[i], (int8_t) (i * oscillator[i]));
            }

            ASSERT_IS_TRUE(multiplier.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_multiplier_with_reader_and_writer()
        {
            TestWriter<int8_t> writer(5);
            HLocalOscillator<float> localOscillator(8000, 1000);
            HMultiplier<int8_t> multiplier(writer.Writer(), 8000, 1000, 10, 5);

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

} hmultiplier_test;
