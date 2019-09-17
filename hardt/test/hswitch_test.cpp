#include <stdio.h>
#include <iostream>

#include "test.h"

class HSwitch_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_read);
            UNITTEST(test_write);
            UNITTEST(test_writerConsumer);
        }

        const char* name()
        {
            return "HSwitch";
        }

    private:


        template <class T>
        class TestReader : public HReader<T>
        {
            public:

                int8_t *_data;

                TestReader(int8_t* data):
                    _data(data)
                {}

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, _data, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int8_t *_received;

                TestWriter(int8_t* received):
                    _received(received)
                {}

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) _received, src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestThroughputWriter : public HWriter<T>, public HWriterConsumer<T>
        {
            private:

                HWriter<T>* _writer;

            public:

                int Write(T* src, size_t blocksize)
                {
                    return _writer->Write(src, blocksize);
                }

                virtual void SetWriter(HWriter<T>* writer)
                {
                    _writer = writer;
                }
        };

        void test_read()
        {
            int8_t data0[] = {1, 2, 3, 4, 5};
            int8_t data1[] = {0, 2, 4, 6, 8};
            int8_t data2[] = {1, 3, 5, 7, 9};
            int8_t data3[] = {5, 4, 3, 2, 1};

            TestReader<int8_t> rd0(data0); // bypass (position 0)
            TestReader<int8_t> rd1(data1);
            TestReader<int8_t> rd2(data2);
            TestReader<int8_t> rd3(data3);

            HSwitch<int8_t> sw(&rd0, 5);

            sw.Add(&rd1);
            sw.Add(&rd2);
            sw.Add(&rd3);

            int8_t received[5];

            // Initial position should be bypass mode
            ASSERT_IS_EQUAL(sw.Read(received, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data0, 5 * sizeof(int8_t)), 0);

            // Check each position
            sw.SetPosition(1);
            ASSERT_IS_EQUAL(sw.Read(received, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data1, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(2);
            ASSERT_IS_EQUAL(sw.Read(received, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data2, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(3);
            ASSERT_IS_EQUAL(sw.Read(received, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data3, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(0);
            ASSERT_IS_EQUAL(sw.Read(received, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data0, 5 * sizeof(int8_t)), 0);
        }

        void test_write()
        {
            int8_t data[] = {1, 2, 3, 4, 5};
            int8_t zero[] = {0, 0, 0, 0, 0};

            int8_t received0[5];
            int8_t received1[5];
            int8_t received2[5];
            int8_t received3[5];

            TestWriter<int8_t> wr0(received0); // bypass (position 0)
            TestWriter<int8_t> wr1(received1);
            TestWriter<int8_t> wr2(received2);
            TestWriter<int8_t> wr3(received3);

            HSwitch<int8_t> sw(&wr0, 5);
            sw.Add(&wr1);
            sw.Add(&wr2);
            sw.Add(&wr3);

            // Initial position should be bypass mode
            memset((void*) received0, 0, 5 * sizeof(int8_t));
            memset((void*) received1, 0, 5 * sizeof(int8_t));
            memset((void*) received2, 0, 5 * sizeof(int8_t));
            memset((void*) received3, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received0, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received1, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received2, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received3, (void*) zero, 5 * sizeof(int8_t)), 0);

            // Check each position
            sw.SetPosition(1);
            memset((void*) received0, 0, 5 * sizeof(int8_t));
            memset((void*) received1, 0, 5 * sizeof(int8_t));
            memset((void*) received2, 0, 5 * sizeof(int8_t));
            memset((void*) received3, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received0, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received1, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received2, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received3, (void*) zero, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(2);
            memset((void*) received0, 0, 5 * sizeof(int8_t));
            memset((void*) received1, 0, 5 * sizeof(int8_t));
            memset((void*) received2, 0, 5 * sizeof(int8_t));
            memset((void*) received3, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received0, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received1, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received2, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received3, (void*) zero, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(3);
            memset((void*) received0, 0, 5 * sizeof(int8_t));
            memset((void*) received1, 0, 5 * sizeof(int8_t));
            memset((void*) received2, 0, 5 * sizeof(int8_t));
            memset((void*) received3, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received0, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received1, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received2, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received3, (void*) data, 5 * sizeof(int8_t)), 0);

            // Switch back to bypass position
            sw.SetPosition(0);
            memset((void*) received0, 0, 5 * sizeof(int8_t));
            memset((void*) received1, 0, 5 * sizeof(int8_t));
            memset((void*) received2, 0, 5 * sizeof(int8_t));
            memset((void*) received3, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received0, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received1, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received2, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) received3, (void*) zero, 5 * sizeof(int8_t)), 0);
        }

        void test_writerConsumer()
        {
            int8_t data[] = {1, 2, 3, 4, 5};
            int8_t expected1[] = {2, 4, 6, 8, 10};
            int8_t expected2[] = {3, 6, 9, 12, 15};
            int8_t expected3[] = {4, 8, 12, 16, 20};
            int8_t zero[] = {0, 0, 0, 0, 0};
            int8_t received[5];

            TestThroughputWriter<int8_t> upstream;

            HSwitch<int8_t> sw(upstream.Consumer(), 5);

            HGain<int8_t> wr1(sw.Consumer(), 2, 5);
            HGain<int8_t> wr2(sw.Consumer(), 3, 5);
            HGain<int8_t> wr3(sw.Consumer(), 4, 5);

            sw.Add(wr1.Writer());
            sw.Add(wr2.Writer());
            sw.Add(wr3.Writer());

            // Downstream writer, simulate the consumer interface pattern
            TestWriter<int8_t> downstream(received); // bypass (position 0)
            sw.SetWriter(&downstream);

            // Initial position should be bypass mode
            memset((void*) received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data, 5 * sizeof(int8_t)), 0);

            // Check each position
            sw.SetPosition(1);
            memset((void*) received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected1, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(2);
            memset((void*) received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected2, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(3);
            memset((void*) received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected3, 5 * sizeof(int8_t)), 0);

            // Switch back to bypass position
            sw.SetPosition(0);
            memset((void*) received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) data, 5 * sizeof(int8_t)), 0);
       }

} hswitch_test;