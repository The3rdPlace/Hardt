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

        void test_read()
        {
            int8_t data0[] = {1, 2, 3, 4, 5};
            int8_t data1[] = {0, 2, 4, 6, 8};
            int8_t data2[] = {1, 3, 5, 7, 9};
            int8_t data3[] = {5, 4, 3, 2, 1};

            TestReader<int8_t> rd0(data0, 5); // bypass (position 0)
            TestReader<int8_t> rd1(data1, 5);
            TestReader<int8_t> rd2(data2, 5);
            TestReader<int8_t> rd3(data3, 5);

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

            TestWriter<int8_t> wr0(5); // bypass (position 0)
            TestWriter<int8_t> wr1(5);
            TestWriter<int8_t> wr2(5);
            TestWriter<int8_t> wr3(5);

            HSwitch<int8_t> sw(wr0.Writer(), 5);
            sw.Add(&wr1);
            sw.Add(&wr2);
            sw.Add(&wr3);

            // Initial position should be bypass mode
            memset((void*) wr0.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr1.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr2.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr3.Received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) wr0.Received, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr1.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr2.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr3.Received, (void*) zero, 5 * sizeof(int8_t)), 0);

            // Check each position
            sw.SetPosition(1);
            memset((void*) wr0.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr1.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr2.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr3.Received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) wr0.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr1.Received, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr2.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr3.Received, (void*) zero, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(2);
            memset((void*) wr0.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr1.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr2.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr3.Received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) wr0.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr1.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr2.Received, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr3.Received, (void*) zero, 5 * sizeof(int8_t)), 0);

            sw.SetPosition(3);
            memset((void*) wr0.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr1.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr2.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr3.Received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) wr0.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr1.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr2.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr3.Received, (void*) data, 5 * sizeof(int8_t)), 0);

            // Switch back to bypass position
            sw.SetPosition(0);
            memset((void*) wr0.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr1.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr2.Received, 0, 5 * sizeof(int8_t));
            memset((void*) wr3.Received, 0, 5 * sizeof(int8_t));
            ASSERT_IS_EQUAL(sw.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) wr0.Received, (void*) data, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr1.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr2.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr3.Received, (void*) zero, 5 * sizeof(int8_t)), 0);
        }

        void test_writerConsumer()
        {
            int8_t data[] = {1, 2, 3, 4, 5};
            int8_t expected1[] = {2, 4, 6, 8, 10};
            int8_t expected2[] = {3, 6, 9, 12, 15};
            int8_t expected3[] = {4, 8, 12, 16, 20};
            int8_t zero[] = {0, 0, 0, 0, 0};
            int8_t received[5];

            // Upstream writer
            TestWriter<int8_t> upstream(5);

            // The switch
            HSwitch<int8_t> sw(upstream.Consumer(), 5);

            // Downstream writer, simulate the consumer interface pattern
            TestWriter<int8_t> downstream(sw.Consumer(), 5); // bypass (position 0)

            // Add writers to the switch
            HGain<int8_t> wr1(downstream.Writer(), 2, 5);
            sw.Add(wr1.Writer());
            HGain<int8_t> wr2(downstream.Writer(), 3, 5);
            sw.Add(wr2.Writer());
            HGain<int8_t> wr3(downstream.Writer(), 4, 5);
            sw.Add(wr3.Writer());

            // Initial position should be bypass mode
            ASSERT_IS_EQUAL(upstream.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) downstream.Received, (void*) data, 5 * sizeof(int8_t)), 0);

            // Check each position
            sw.SetPosition(1);
            ASSERT_IS_EQUAL(upstream.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) downstream.Received, (void*) expected1, 5 * sizeof(int8_t)), 0);
            sw.SetPosition(2);
            ASSERT_IS_EQUAL(upstream.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) downstream.Received, (void*) expected2, 5 * sizeof(int8_t)), 0);
            sw.SetPosition(3);
            ASSERT_IS_EQUAL(upstream.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) downstream.Received, (void*) expected3, 5 * sizeof(int8_t)), 0);

            // Switch back to bypass position
            sw.SetPosition(0);
            ASSERT_IS_EQUAL(upstream.Write(data, 5), 5);
            ASSERT_IS_EQUAL(memcmp((void*) downstream.Received, (void*) data, 5 * sizeof(int8_t)), 0);
       }

} hswitch_test;