#include <stdio.h>
#include <iostream>

#include "test.h"

class HConverter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_converter_int8_int8_read);
            UNITTEST(test_converter_int8_int8_write);
            UNITTEST(test_converter_int8_int8_read_overflow);
            UNITTEST(test_converter_int8_int8_write_overflow);
            UNITTEST(test_converter_int8_int16_read);
            UNITTEST(test_converter_int8_int16_write);
        }

        const char* name()
        {
            return "HConverter";
        }

    private:

    public:

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                T* Output;

                TestWriter(T* output):
                    Output(output)
                {
                }

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) Output, (void*) src, blocksize * sizeof(T));
                    return blocksize;
                }

                /** Execute or carry through a command */
                bool Command(HCommand* command) {
                    // No ruther propagation of commands
                    return true;
                }
        };

        template <class T>
        class TestReader : public HReader<T>
        {
            public:

                T* Input;

                TestReader(T* input):
                    Input(input)
                {
                }

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, (void*) Input, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class Tin, class Tout>
        class TestConverter : public HConverter<Tin, Tout>
        {
            private:

                int _factor;

                int Convert(Tin* src, Tout* dest, size_t blocksize)
                {
                    for( int i = 0; i < blocksize; i++ )
                    {
                        dest[i] = src[i] * _factor;
                    }
                    return blocksize;
                }

            public:

                TestConverter(HReader<Tin>* reader, size_t blocksize, int factor):
                    HConverter<Tin, Tout>(reader, blocksize),
                    _factor(factor)
                {}

                TestConverter(HWriter<Tout>* writer, size_t blocksize, int factor):
                    HConverter<Tin, Tout>(writer, blocksize),
                    _factor(factor)
                {}
        };

        void test_converter_int8_int8_read()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {2, 4, 6, 8, 10, 12};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input);
            TestConverter<int8_t, int8_t> converter(&reader, 6, 2);

            converter.Read(output, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }
        }

        void test_converter_int8_int8_write()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {3, 6, 9, 12, 15, 18};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int8_t> writer(output);
            TestConverter<int8_t, int8_t> converter(&writer, 6, 3);

            converter.Write(input, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }
        }

        void test_converter_int8_int8_read_overflow()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {-24, -48, -72, -96, -120, 112};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input);
            TestConverter<int8_t, int8_t> converter(&reader, 6, 1000);

            converter.Read(output, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }
        }

        void test_converter_int8_int8_write_overflow()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {-48, -96, 112, 64, 16, -32};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int8_t> writer(output);
            TestConverter<int8_t, int8_t> converter(&writer, 6, 2000);

            converter.Write(input, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }
        }

        void test_converter_int8_int16_read()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int16_t expected[6] = {3000, 6000, 9000, 12000, 15000, 18000};
            int16_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input);
            TestConverter<int8_t, int16_t> converter(&reader, 6, 3000);

            converter.Read(output, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }
        }

        void test_converter_int8_int16_write()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int16_t expected[6] = {4000, 8000, 12000, 16000, 20000, 24000};
            int16_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestWriter<int16_t> writer(output);
            TestConverter<int8_t, int16_t> converter(&writer, 6, 4000);

            converter.Write(input, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }
        }

} hconverter_test;
