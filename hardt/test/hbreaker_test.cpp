#include <stdio.h>
#include <iostream>

#include "test.h"

class HBreaker_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_off_as_writer);
            UNITTEST(test_off_as_reader);

            UNITTEST(test_start_stop_as_writer);
            UNITTEST(test_start_stop_as_reader);
        }

        const char* name()
        {
            return "HBreaker";
        }

    private:

        int8_t expected[6] = {0, 0, 0, 0, 0, 0};

        void test_off_as_writer()
        {
            TestWriter<int8_t> wr(8);
            int8_t input[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            HBreaker<int8_t> off(wr.Writer(), false, 6);

            ASSERT_IS_EQUAL(off.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) input, 6 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(wr.Writes, 1);

            off.SetOff(true);
            ASSERT_IS_EQUAL(off.Write(input, 6), 6);
            ASSERT_IS_EQUAL(wr.Writes, 1);

            ASSERT_IS_TRUE(off.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_off_as_reader()
        {
            int8_t output[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            TestReader<int8_t> rd(output, 8);
            HBreaker<int8_t> off(&rd, false, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(off.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) output, 6 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(rd.Reads, 1);

            off.SetOff(true);
            ASSERT_IS_EQUAL(off.Read(received, 6), 0);

            ASSERT_IS_TRUE(off.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_start_stop_as_writer()
        {
            // Initial start not-offd (false)
            TestWriter<int8_t> wr(8);
            HBreaker<int8_t> off(wr.Writer(), false, 6);

            off.Start(); // start = 1
            off.Start(); // start = 2
            off.Stop();  // stop = 1
            off.Start(); // start = 3
            ASSERT_IS_EQUAL(wr.Started, 3);
            ASSERT_IS_EQUAL(wr.Stopped, 1);

            off.SetOff(true); // stop = 2
            ASSERT_IS_EQUAL(wr.Started, 3);
            ASSERT_IS_EQUAL(wr.Stopped, 2);

            off.Start(); // start = 3
            off.Stop(); // stop =2
            ASSERT_IS_EQUAL(wr.Started, 3);
            ASSERT_IS_EQUAL(wr.Stopped, 2);

            off.SetOff(false); // start = 4
            ASSERT_IS_EQUAL(wr.Started, 4);
            ASSERT_IS_EQUAL(wr.Stopped, 2);

            off.Start(); // start = 5
            off.Stop(); // stop = 3
            ASSERT_IS_EQUAL(wr.Started, 5);
            ASSERT_IS_EQUAL(wr.Stopped, 3);
        }

        void test_start_stop_as_reader()
        {
            // Initial start not-offd (false)
            int8_t output[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            TestReader<int8_t> rd(output, 8);
            HBreaker<int8_t> off(&rd, false, 6);

            off.Start(); // start = 1
            off.Start(); // start = 2
            off.Stop();  // stop = 1
            off.Start(); // start = 3
            ASSERT_IS_EQUAL(rd.Started, 3);
            ASSERT_IS_EQUAL(rd.Stopped, 1);

            off.SetOff(true); // stop = 2
            ASSERT_IS_EQUAL(rd.Started, 3);
            ASSERT_IS_EQUAL(rd.Stopped, 2);

            off.Start(); // start = 3
            off.Stop(); // stop =2
            ASSERT_IS_EQUAL(rd.Started, 3);
            ASSERT_IS_EQUAL(rd.Stopped, 2);

            off.SetOff(false); // start = 4
            ASSERT_IS_EQUAL(rd.Started, 4);
            ASSERT_IS_EQUAL(rd.Stopped, 2);

            off.Start(); // start = 5
            off.Stop(); // stop = 3
            ASSERT_IS_EQUAL(rd.Started, 5);
            ASSERT_IS_EQUAL(rd.Stopped, 3);
        }
} hbreaker_test;