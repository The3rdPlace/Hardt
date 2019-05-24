#include <stdio.h>
#include <iostream>

#include "test.h"

class HConverter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_converter);
        }

        const char* name()
        {
            return "HConverter";
        }

    private:

        template <class T>
        class TestConverter : public HConverter<T>
        {
            private:

                int Convert(T* src, size_t blocksize)
                {
                    counter++;

                    for( int i = 0; i < blocksize; i++ )
                    {
                        result += src[i];
                    }

                    return blocksize;
                }

            public:

                TestConverter():
                    counter(0),
                    result(0)
                {}

                int counter;
                int result;
        };

        static bool readyCalled;

        static void ready(TestConverter<int8_t>* converter)
        {
            readyCalled = true;
        }

        void test_converter()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};
            TestConverter<int8_t> converter;

            converter.Write(input, 2);
            converter.Write(&input[2], 2);

            ASSERT_IS_EQUAL(converter.result, 10);
            ASSERT_IS_EQUAL(converter.counter, 2);

            converter.Write(&input[4], 2);
            ASSERT_IS_EQUAL(converter.result, 21);
            ASSERT_IS_EQUAL(converter.counter, 3);
        }

} hconverter_test;

bool HConverter_Test::readyCalled = false;