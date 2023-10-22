#include <iostream>

#include "test.h"

void isReady(int32_t*, size_t size)
{
    std::cout << "READY" << std::endl;
}

class HOutput_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_output);
            UNITTEST(test_output_ready);
            UNITTEST(test_output_ready_buffer);
            UNITTEST(test_output_ready_value);
            UNITTEST(test_chunked_output);
        }

        const char* name() override
        {
            return "HOutput";
        }

    private:

    public:

        template <class T>
        class TestOutput : public HOutput<T, int32_t>
        {
            private:

                int Output(T* src, size_t blocksize) override
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

                explicit TestOutput(std::string id):
                    HOutput<T, int32_t>(id),
                    counter(0),
                    result(0)
                {}

                TestOutput(std::string id, std::function<void()> ready):
                    HOutput<T, int32_t>(id, ready),
                    counter(0),
                    result(0)
                {}

                TestOutput(std::string id, std::function<void(int32_t*, size_t)> ready):
                    HOutput<T, int32_t>(id, ready),
                    counter(0),
                    result(0)
                {}

                TestOutput(std::string id, std::function<void(int32_t)> ready):
                    HOutput<T, int32_t>(id, ready),
                    counter(0),
                    result(0)
                {}

                TestOutput(std::string id, int chunksize):
                    HOutput<T, int32_t>(id, 2),
                    counter(0),
                    result(0)
                {}

                int counter;
                int result;

                /** Execute or carry through a command */
                bool Command(HCommand* command) {
                    // No ruther propagation of commands
                    return true;
                }
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

        void test_output()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            TestOutput<int8_t> output("test_output");
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            output.Write(input, 2);
            output.Write(&input[2], 2);

            ASSERT_IS_EQUAL(output.result, 10);
            ASSERT_IS_EQUAL(output.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);

            output.Write(&input[4], 2);
            ASSERT_IS_EQUAL(output.result, 21);
            ASSERT_IS_EQUAL(output.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

        void test_output_ready()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            std::function<void()> func = std::bind( &HOutput_Test::ready, this);
            TestOutput<int8_t> output("houtput_test_output_ready", func);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            output.Write(input, 2);
            output.Write(&input[2], 2);

            ASSERT_IS_EQUAL(output.result, 10);
            ASSERT_IS_EQUAL(output.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 1);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);

            output.Write(&input[4], 2);
            ASSERT_IS_EQUAL(output.result, 21);
            ASSERT_IS_EQUAL(output.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 1);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

        void test_output_ready_buffer()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            using std::placeholders::_1;
            using std::placeholders::_2;
            std::function<void(int32_t*, size_t)> func = std::bind( &HOutput_Test::readyBuffer, this, _1, _2);
            TestOutput<int8_t> output("houtput_test_ready_buffer", func);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            output.Write(input, 2);
            output.Write(&input[2], 2);

            ASSERT_IS_EQUAL(output.result, 10);
            ASSERT_IS_EQUAL(output.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 1);
            ASSERT_IS_EQUAL(readyValueCalled, 0);

            output.Write(&input[4], 2);
            ASSERT_IS_EQUAL(output.result, 21);
            ASSERT_IS_EQUAL(output.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 1);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

        void test_output_ready_value()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            using std::placeholders::_1;
            std::function<void(int32_t)> func = std::bind( &HOutput_Test::readyValue, this, _1);
            TestOutput<int8_t> output("houtput_test_ready_value", func);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            output.Write(input, 2);
            output.Write(&input[2], 2);

            ASSERT_IS_EQUAL(output.result, 10);
            ASSERT_IS_EQUAL(output.counter, 2);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 1);

            output.Write(&input[4], 2);
            ASSERT_IS_EQUAL(output.result, 21);
            ASSERT_IS_EQUAL(output.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 1);
        }

        void test_chunked_output()
        {
            int8_t input[8] = {1, 2, 3, 4, 5, 6};

            TestOutput<int8_t> output("houtput_test_chunked_output", 2);
            readyCalled = 0;
            readyBufferCalled = 0;
            readyValueCalled = 0;

            output.Write(input, 6);

            ASSERT_IS_EQUAL(output.result, 21);
            ASSERT_IS_EQUAL(output.counter, 3);

            ASSERT_IS_EQUAL(readyCalled, 0);
            ASSERT_IS_EQUAL(readyBufferCalled, 0);
            ASSERT_IS_EQUAL(readyValueCalled, 0);
        }

} hOutput_test;
