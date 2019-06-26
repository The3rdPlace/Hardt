#include <stdio.h>
#include <iostream>

#include "test.h"

class HLinearMixer_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_mixer);
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

                char data[5];

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

        void test_mixer()
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
} hlinearmixer_test;
