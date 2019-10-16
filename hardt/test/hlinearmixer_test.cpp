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

        template <class T>
        class TestReader : public HReader<T>
        {
            private:

                T data[5];

            public:

                TestReader()
                {
                    for( int i = 0; i < 5; i++ )
                    {
                        data[i] = i;
                    }
                }

                int Read(T* dest, size_t blocksize)
                {
                    memset((void*) dest, 0, sizeof(T) * blocksize);
                    if( blocksize != 5 )
                    {
                        return 0;
                    }

                    memcpy((void*) dest, (void*) data, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                T Received[5];

            public:

                int Write(T* src, size_t blocksize)
                {
                    if( blocksize != 5 )
                    {
                        return 0;
                    }

                    memcpy((void*) Received, (void*) src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_mixer_with_readers()
        {
            TestReader<int8_t> reader_1;
            TestReader<int8_t> reader_2;
            HLinearMixer<int8_t> mixer(&reader_1, &reader_2, 5);

            int8_t output[5];
            ASSERT_IS_EQUAL(mixer.Read(output, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(output[i], (int8_t) (i + i));
            }
        }

        void test_mixer_with_reader_and_writer()
        {
            TestReader<int8_t> reader;
            TestWriter<int8_t> writer;
            HLinearMixer<int8_t> mixer(&reader, &writer, 5);

            int8_t input[5] = {4, 3, 2, 1, 0};
            int8_t expected[5] = {4, 4, 4, 4, 4};
            ASSERT_IS_EQUAL(mixer.Write(input, 5), 5);

            for(int i = 0; i < 5; i++)
            {
                ASSERT_IS_EQUAL(writer.Received[i], expected[i]);
            }
        }

} hlinearmixer_test;
