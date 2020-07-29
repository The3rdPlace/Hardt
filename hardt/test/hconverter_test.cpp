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
            UNITTEST(test_converter_int8_int8_read_1_2);
            UNITTEST(test_converter_int8_int8_write_1_2);
            UNITTEST(test_converter_int8_int8_read_2_1);
            UNITTEST(test_converter_int8_int8_write_2_1);
        }

        const char* name()
        {
            return "HConverter";
        }

    private:

    public:

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
                    HConverter<Tin, Tout>(reader, blocksize, blocksize),
                    _factor(factor)
                {}

                TestConverter(HWriter<Tout>* writer, size_t blocksize, int factor):
                    HConverter<Tin, Tout>(writer, blocksize, blocksize),
                    _factor(factor)
                {}
        };

        template <class Tin, class Tout>
        class TestConverter_1_2 : public HConverter<Tin, Tout>
        {
            private:

                int _factor;

                int Convert(Tin* src, Tout* dest, size_t blocksize)
                {
                    for( int i = 0; i < blocksize * 2; i++ )
                    {
                        dest[i] = src[i / 2] * _factor;
                    }
                    return blocksize * 2;
                }

            public:

                TestConverter_1_2(HReader<Tin>* reader, size_t blocksize, int factor):
                        HConverter<Tin, Tout>(reader, blocksize, blocksize * 2),
                        _factor(factor)
                {}

                TestConverter_1_2(HWriter<Tout>* writer, size_t blocksize, int factor):
                        HConverter<Tin, Tout>(writer, blocksize, blocksize * 2),
                        _factor(factor)
                {}
        };

    template <class Tin, class Tout>
    class TestConverter_2_1 : public HConverter<Tin, Tout>
    {
        private:

            int _factor;

            int Convert(Tin* src, Tout* dest, size_t blocksize)
            {
                for( int i = 0; i < blocksize; i++ )
                {
                    dest[i / 2] = src[i] * _factor;
                }
                return blocksize / 2;
            }

        public:

            TestConverter_2_1(HReader<Tin>* reader, size_t blocksize, int factor):
                    HConverter<Tin, Tout>(reader, blocksize, blocksize / 2),
                    _factor(factor)
            {}

            TestConverter_2_1(HWriter<Tout>* writer, size_t blocksize, int factor):
                    HConverter<Tin, Tout>(writer, blocksize, blocksize / 2),
                    _factor(factor)
            {}
    };

    void test_converter_int8_int8_read()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {2, 4, 6, 8, 10, 12};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input, 6);
            TestConverter<int8_t, int8_t> converter(&reader, 6, 2);

            converter.Read(output, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_converter_int8_int8_write()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {3, 6, 9, 12, 15, 18};

            TestWriter<int8_t> writer(6);
            TestConverter<int8_t, int8_t> converter(&writer, 6, 3);

            converter.Write(input, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

        void test_converter_int8_int8_read_overflow()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {-24, -48, -72, -96, -120, 112};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input, 6);
            TestConverter<int8_t, int8_t> converter(&reader, 6, 1000);

            converter.Read(output, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_converter_int8_int8_write_overflow()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {-48, -96, 112, 64, 16, -32};

            TestWriter<int8_t> writer(6);
            TestConverter<int8_t, int8_t> converter(&writer, 6, 2000);

            converter.Write(input, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

        void test_converter_int8_int16_read()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int16_t expected[6] = {3000, 6000, 9000, 12000, 15000, 18000};
            int16_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input, 6);
            TestConverter<int8_t, int16_t> converter(&reader, 6, 3000);

            converter.Read(output, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_converter_int8_int16_write()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int16_t expected[6] = {4000, 8000, 12000, 16000, 20000, 24000};

            TestWriter<int16_t> writer(6);
            TestConverter<int8_t, int16_t> converter(&writer, 6, 4000);

            converter.Write(input, 6);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

        void test_converter_int8_int8_read_1_2()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[12] = {2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12};
            int8_t output[12] = {0, 0 ,0 ,0 ,0 ,0, 0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input, 6);
            TestConverter_1_2<int8_t, int8_t> converter(&reader, 6, 2);

            ASSERT_IS_EQUAL(converter.Read(output, 12), 12);
            ASSERT_IS_EQUAL(reader.Reads, 1);
            ASSERT_IS_EQUAL(reader.Samples, 6);

            for(int i = 0; i < 12; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_converter_int8_int8_write_1_2()
        {
            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t expected[12] = {3, 3, 6, 6, 9, 9, 12, 12, 15, 15, 18, 18};

            TestWriter<int8_t> writer(12);
            TestConverter_1_2<int8_t, int8_t> converter(&writer, 6, 3);

            ASSERT_IS_EQUAL(converter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(writer.Writes, 1);
            ASSERT_IS_EQUAL(writer.Samples, 12);

            for(int i = 0; i < 12; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

        void test_converter_int8_int8_read_2_1()
        {
            int8_t input[12] = {1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6};
            int8_t expected[6] = {4, 8, 12, 4, 8, 12};
            int8_t output[6] = {0, 0 ,0 ,0 ,0 ,0};

            TestReader<int8_t> reader(input, 6);
            TestConverter_2_1<int8_t, int8_t> converter(&reader, 12, 2);

            ASSERT_IS_EQUAL(converter.Read(output, 6), 6);
            ASSERT_IS_EQUAL(reader.Reads, 1);
            ASSERT_IS_EQUAL(reader.Samples, 12);

            for(int i = 0; i < 6; i++)
            {
                ASSERT_IS_EQUAL(output[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(reader.Commands, 1);
        }

        void test_converter_int8_int8_write_2_1()
        {
            int8_t input[12] = {1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6};
            int8_t expected[12] = {6, 12, 18, 6, 12, 18};

            TestWriter<int8_t> writer(12);
            TestConverter_2_1<int8_t, int8_t> converter(&writer, 12, 3);

            ASSERT_IS_EQUAL(converter.Write(input, 12), 12);
            ASSERT_IS_EQUAL(writer.Writes, 1);
            ASSERT_IS_EQUAL(writer.Samples, 6);

            for(int i = 0; i < 12; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }

            ASSERT_IS_TRUE(converter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer.Commands, 1);
        }

} hconverter_test;
