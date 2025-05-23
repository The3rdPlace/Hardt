#include <vector>

#include "test.h"

class HSplitter_Test: public Test
{
    public:

        void run() override
        {
            UNITTEST(test_write);
            UNITTEST(test_write_with_vector);
            UNITTEST(test_ignore_multiple_setwriter_calls);
        }

        const char* name() override
        {
            return "HSplitter";
        }

    private:

        /*template <class T>
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

             Execute or carry through a command
            bool Command(HCommand* command) {
                // No ruther propagation of commands
                return true;
            }
        };*/

        void test_write()
        {
            TestWriter<int8_t> writer1 = TestWriter<int8_t>("hsplitter_test_testwriter_1", 10);
            TestWriter<int8_t> writer2 = TestWriter<int8_t>("hsplitter_test_testwriter_2", 10);

            HSplitter<int8_t> splitter("hsplitter_test_write", &writer1, &writer2);
            int8_t input[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(splitter.Write(input, 10), 10);

            ASSERT_IS_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);
        }

        void test_write_with_vector()
        {
            TestWriter<int8_t> writer1 = TestWriter<int8_t>("hsplitter_test_testwriter_1", 10);
            TestWriter<int8_t> writer2 = TestWriter<int8_t>("hsplitter_test_testwriter_2", 10);
            TestWriter<int8_t> writer3 = TestWriter<int8_t>("hsplitter_test_testwriter_3", 10);

            std::vector< HWriter<int8_t>* > writers;
            writers.push_back(&writer1);
            writers.push_back(&writer2);
            writers.push_back(&writer3);

            HSplitter<int8_t> splitter("hsplitter_test_write_with_vector", writers);
            int8_t input[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_NOT_EQUAL(memcmp((void*) writer3.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(splitter.Write(input, 10), 10);

            ASSERT_IS_EQUAL(memcmp((void*) writer1.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer2.Received, (void*) input, 10 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) writer3.Received, (void*) input, 10 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(splitter.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer1.Commands, 1);
            ASSERT_IS_EQUAL(writer2.Commands, 1);
            ASSERT_IS_EQUAL(writer3.Commands, 1);
        }

        void test_ignore_multiple_setwriter_calls()
        {
            TestWriter<int8_t> Parent("hsplitter_test_testwriter_parent", 10);
            TestWriter<int8_t> writer1("hsplitter_test_testwriter_1", 10);
            TestWriter<int8_t> writer2("hsplitter_test_testwriter_2", 10);

            HSplitter<int8_t> splitter("hsplitter_test_ignore_multiple_setwriter_calls", Parent.Consumer());
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

            ASSERT_IS_TRUE(Parent.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(writer1.Commands, 1);
            ASSERT_IS_EQUAL(writer2.Commands, 1);
        }

} hsplitter_test;