#include <vector>

#include "test.h"

class HSplitter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_write);
            UNITTEST(test_write_with_vector);
        }

        const char* name()
        {
            return "HSplitter";
        }

    private:

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                T* Received;

                TestWriter(size_t blocksize)
                {
                    Received = new T[blocksize];
                    memset((void*) Received, 0, blocksize * sizeof(T));
                }

                ~TestWriter()
                {
                    delete[] Received;
                }

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) Received, (void*) src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_write()
        {
            TestWriter<int8_t> writer1 = TestWriter<int8_t>(10);
            TestWriter<int8_t> writer2 = TestWriter<int8_t>(10);

            HSplitter<int8_t> splitter(&writer1, &writer2);
            int8_t input[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(splitter.Write(input, 10), 10);

            ASSERT_IS_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);
        }

        void test_write_with_vector()
        {
            TestWriter<int8_t> writer1 = TestWriter<int8_t>(10);
            TestWriter<int8_t> writer2 = TestWriter<int8_t>(10);
            TestWriter<int8_t> writer3 = TestWriter<int8_t>(10);

            std::vector< HWriter<int8_t>* > writers;
            writers.push_back(&writer1);
            writers.push_back(&writer2);
            writers.push_back(&writer3);

            HSplitter<int8_t> splitter(writers);
            int8_t input[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer3.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(splitter.Write(input, 10), 10);

            ASSERT_IS_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer3.Received, (void*) input, 10 * sizeof(int8_t)), 0);
        }

} hsplitter_test;