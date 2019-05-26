#include <stdio.h>
#include <iostream>

#include "test.h"

void isReady(int32_t*, size_t size)
{
    std::cout << "READY" << std::endl;
}

class HConverter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_converter);
            UNITTEST(test_converter_ready);
            UNITTEST(test_converter_ready_buffer);
            UNITTEST(test_converter_ready_value);
            UNITTEST(test_chunked_converter);
        }

        const char* name()
        {
            return "HConverter";
        }

    private:

    public:

        template <class T>
        class TestConverter : public HConverter<T, int32_t>
        {
            private:

                int Convert(T* src, size_t blocksize)
                {
                    counter++;

                    for( int i = 0; i < blocksize; i++ )
                    {
                        result += src[i];
                    }

                    if( counter % 2 == 0 )
                    {
                        int32_t result[] = { 21, 22 };
                        this->Ready(result, 2);
                        this->Ready(23);
                        this->Ready();
                    }

                    return blocksize;
                }

            public:

                TestConverter():
                    counter(0),
                    result(0)
                {}

                TestConverter(std::function<void()> ready):
                    HConverter<T, int32_t>(ready),
                    counter(0),
                    result(0)
                {}

                TestConverter(std::function<void(int32_t*, size_t)> ready):
                    HConverter<T, int32_t>(ready),
                    counter(0),
                    result(0)
                {}

                TestConverter(std::function<void(int32_t)> ready):
                    HConverter<T, int32_t>(ready),
                    counter(0),
                    result(0)
                {}

                TestConverter(int chunksize):
                    HConverter<T, int32_t>(2),
                    counter(0),
                    result(0)
                {}

                int counter;
                int result;
        };

        int readyCalled;
        int readyBufferCalled;
        int readyValueCalled;

        void ready()
        {
            readyCalled++;
        }

        void readyBuffer(int32_t* data, size_t size)
        {
            readyBufferCalled++;
            ASSERT_IS_EQUAL((int) size, 2);
            ASSERT_IS_EQUAL(data[0], 21);
            ASSERT_IS_EQUAL(data[1], 22);
        }

        void readyValue(int32_t value)
        {
            readyValueCalled++;
            ASSERT_IS_EQUAL(value, 23);
        }

        void test_converter()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            TestConverter<int8_t> converter;
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            converter.Write(input, 2);
            converter.Write(&input[2], 2);

            ASSERT_IS_EQUAL(converter.result, 10);
            ASSERT_IS_EQUAL(converter.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);

            converter.Write(&input[4], 2);
            ASSERT_IS_EQUAL(converter.result, 21);
            ASSERT_IS_EQUAL(converter.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

        void test_converter_ready()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            std::function<void()> func = std::bind( &HConverter_Test::ready, this);
            TestConverter<int8_t> converter(func);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            converter.Write(input, 2);
            converter.Write(&input[2], 2);

            ASSERT_IS_EQUAL(converter.result, 10);
            ASSERT_IS_EQUAL(converter.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 1);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);

            converter.Write(&input[4], 2);
            ASSERT_IS_EQUAL(converter.result, 21);
            ASSERT_IS_EQUAL(converter.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 1);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

        void test_converter_ready_buffer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<void(int32_t*, size_t)> func = std::bind( &HConverter_Test::readyBuffer, this, _1, _2);
            TestConverter<int8_t> converter(func);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            converter.Write(input, 2);
            converter.Write(&input[2], 2);

            ASSERT_IS_EQUAL(converter.result, 10);
            ASSERT_IS_EQUAL(converter.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 1);
            ASSERT_IS_EQUAL(readyValueCalled, 0);

            converter.Write(&input[4], 2);
            ASSERT_IS_EQUAL(converter.result, 21);
            ASSERT_IS_EQUAL(converter.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 1);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

        void test_converter_ready_value()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            using std::placeholders::_1;
            std::function<void(int32_t)> func = std::bind( &HConverter_Test::readyValue, this, _1);
            TestConverter<int8_t> converter(func);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            converter.Write(input, 2);
            converter.Write(&input[2], 2);

            ASSERT_IS_EQUAL(converter.result, 10);
            ASSERT_IS_EQUAL(converter.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 1);

            converter.Write(&input[4], 2);
            ASSERT_IS_EQUAL(converter.result, 21);
            ASSERT_IS_EQUAL(converter.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 1);
        }

        void test_chunked_converter()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            TestConverter<int8_t> converter(2);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            converter.Write(input, 6);

            ASSERT_IS_EQUAL(converter.result, 21);
            ASSERT_IS_EQUAL(converter.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

} hconverter_test;
