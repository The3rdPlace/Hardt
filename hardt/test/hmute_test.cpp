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
        }

        const char* name()
        {
            return "HMute";
        }

    private:

        int8_t expected[6] = {0, 0, 0, 0, 0, 0};

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int8_t received[6];

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) received, src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestReader : public HReader<T>
        {
            public:

                int8_t output[8] = {10, 20, 30, 40, 50, 60, 70, 80};

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, output, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_mute_as_writer()
        {
            TestWriter<int8_t> wr;
            int8_t input[8] = {10, 20, 30, 40, 50, 60, 70, 80};
            HMute<int8_t> mute(&wr, false, 6);

            ASSERT_IS_EQUAL(mute.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) input, 6 * sizeof(int8_t)), 0);

            mute.SetMuted(true);
            ASSERT_IS_EQUAL(mute.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected, 6 * sizeof(int8_t)), 0);
        }

        void test_mute_as_reader()
        {
            TestReader<int8_t> rd;
            HMute<int8_t> mute(&rd, false, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(mute.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) rd.output, 6 * sizeof(int8_t)), 0);

            mute.SetMuted(true);
            ASSERT_IS_EQUAL(mute.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);
        }
} hmute_test;