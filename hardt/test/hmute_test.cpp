#include <stdio.h>
#include <iostream>

#include "test.h"

class HMute_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_mute_as_writer);
            UNITTEST(test_mute_as_reader);

            UNITTEST(test_start_stop_as_writer);
            UNITTEST(test_start_stop_as_reader);
        }

        const char* name()
        {
            return "HMute";
        }

    private:

        int8_t expected[6] = {0, 0, 0, 0, 0, 0};

        void test_mute_as_writer()
        {
            TestWriter<int8_t> wr(8);
            int8_t input[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            HMute<int8_t> mute(wr.Writer(), false, 6);

            ASSERT_IS_EQUAL(mute.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) input, 6 * sizeof(int8_t)), 0);

            mute.SetMuted(true);
            ASSERT_IS_EQUAL(mute.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mute.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_mute_as_reader()
        {
            int8_t output[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            TestReader<int8_t> rd(output, 8);
            HMute<int8_t> mute(&rd, false, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(mute.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) output, 6 * sizeof(int8_t)), 0);

            mute.SetMuted(true);
            ASSERT_IS_EQUAL(mute.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_TRUE(mute.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }

        void test_start_stop_as_writer()
        {
            // Initial start not-muted (false)
            TestWriter<int8_t> wr(8);
            HMute<int8_t> mute(wr.Writer(), false, 6);

            mute.Start(); // start = 1
            mute.Start(); // start = 2
            mute.Stop();  // stop = 1
            mute.Start(); // start = 3
            ASSERT_IS_EQUAL(wr.Started, 3);
            ASSERT_IS_EQUAL(wr.Stopped, 1);

            mute.SetMuted(true); // stop = 2
            ASSERT_IS_EQUAL(wr.Started, 3);
            ASSERT_IS_EQUAL(wr.Stopped, 2);

            mute.Start(); // start = 3
            mute.Stop(); // stop =2
            ASSERT_IS_EQUAL(wr.Started, 3);
            ASSERT_IS_EQUAL(wr.Stopped, 2);

            mute.SetMuted(false); // start = 4
            ASSERT_IS_EQUAL(wr.Started, 4);
            ASSERT_IS_EQUAL(wr.Stopped, 2);

            mute.Start(); // start = 5
            mute.Stop(); // stop = 3
            ASSERT_IS_EQUAL(wr.Started, 5);
            ASSERT_IS_EQUAL(wr.Stopped, 3);
        }

        void test_start_stop_as_reader()
        {
            // Initial start not-muted (false)
            int8_t output[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            TestReader<int8_t> rd(output, 8);
            HMute<int8_t> mute(&rd, false, 6);

            mute.Start(); // start = 1
            mute.Start(); // start = 2
            mute.Stop();  // stop = 1
            mute.Start(); // start = 3
            ASSERT_IS_EQUAL(rd.Started, 3);
            ASSERT_IS_EQUAL(rd.Stopped, 1);

            mute.SetMuted(true); // stop = 2
            ASSERT_IS_EQUAL(rd.Started, 3);
            ASSERT_IS_EQUAL(rd.Stopped, 2);

            mute.Start(); // start = 3
            mute.Stop(); // stop =2
            ASSERT_IS_EQUAL(rd.Started, 3);
            ASSERT_IS_EQUAL(rd.Stopped, 2);

            mute.SetMuted(false); // start = 4
            ASSERT_IS_EQUAL(rd.Started, 4);
            ASSERT_IS_EQUAL(rd.Stopped, 2);

            mute.Start(); // start = 5
            mute.Stop(); // stop = 3
            ASSERT_IS_EQUAL(rd.Started, 5);
            ASSERT_IS_EQUAL(rd.Stopped, 3);
        }
} hmute_test;