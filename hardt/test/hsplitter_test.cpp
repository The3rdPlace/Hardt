#include <vector>

#include "test.h"

class HSplitter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_write);
            UNITTEST(test_write_with_vector);
            UNITTEST(test_ignore_multiple_setwriter_calls);
        }

        const char* name()
        {
            return "HSplitter";
        }

    private:

        template <class T>
        class TestWriter : public HWriter<T>, public HWriterConsumer<T>
        {
            public:

                T* Received;
                int Writes;

                TestWriter(size_t blocksize)
                {
                    Received = new T[blocksize];
                    memset((void*) Received, 0, blocksize * sizeof(T));
                    Writes = 0;
                }

                ~TestWriter()
                {
                    delete[] Received;
                }

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) Received, (void*) src, blocksize * sizeof(T));
                    Writes++;
                    return blocksize;
                }

                void SetWriter(HWriter<T>* writer) {}

            /** Execute or carry through a command */
            bool Command(HCommand* command) {
                // No ruther propagation of commands
                return true;
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

        void test_ignore_multiple_setwriter_calls()
        {
            TestWriter<int8_t> Parent(10);
            TestWriter<int8_t> writer1(10);
            TestWriter<int8_t> writer2(10);

            HSplitter<int8_t> splitter(Parent.Consumer());
            int8_t input[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            splitter.SetWriter(&writer1);
            splitter.SetWriter(&writer2);
            splitter.SetWriter(&writer2);

            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(splitter.Write(input, 10), 10);

            ASSERT_IS_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(writer1.Writes, 1);
            ASSERT_IS_EQUAL(writer2.Writes, 1);
        }

} hsplitter_test;