#include <stdio.h>
#include <iostream>

#include "test.h"

class HLinearMixer_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_mixer_with_readers);
            UNITTEST(test_mixer_with_reader_and_writer);
        }

        const char* name()
        {
            return "HLinearMixer";
        }

    public:

        void test_mixer_with_readers()
        {
            int8_t input[5] = {0, 1, 2, 3, 4};
            TestReader<int8_t> reader_1(input, 5);
            TestReader<int8_t> reader_2(input, 5);
            HLinearMixer<int8_t> mixer(&reader_1, &reader_2, 5);

            int8_t output[5];
            ASSERT_IS_EQUAL(mixer.Read(output, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(output[i], (int8_t) (i + i));
            }

            ASSERT_IS_TRUE(mixer.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader_1.Commands, 1);
            ASSERT_IS_EQUAL(reader_2.Commands, 1);
        }

        void test_mixer_with_reader_and_writer()
        {
            int8_t input_1[5] = {0, 1, 2, 3, 4};
            TestReader<int8_t> reader(input_1, 5);
            TestWriter<int8_t> writer(5);
            HLinearMixer<int8_t> mixer(reader.Reader(), writer.Writer(), 5);

            int8_t input_2[5] = {4, 3, 2, 1, 0};
            int8_t expected[5] = {4, 4, 4, 4, 4};
            ASSERT_IS_EQUAL(mixer.Write(input_2, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(mixer.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

} hlinearmixer_test;
