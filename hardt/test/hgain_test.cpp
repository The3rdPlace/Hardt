#include <stdio.h>
#include <iostream>

#include "test.h"

class HGain_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_gain_as_writer);
            UNITTEST(test_gain_as_reader);
        }

        const char* name()
        {
            return "HGain";
        }

    private:

        int8_t expected_2[6] = {2, 4, 6, 8, 10, 12};
        int8_t expected_0_5[6] = {0, 1, 1, 2, 2, 3};
        int8_t expected_neg2[6] = {0, 0, 0, 0, 0, 0};

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
            private:

                int8_t output[8] = {1, 2, 3, 4, 5, 6, 7, 8};

            public:

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, output, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_gain_as_writer()
        {
            TestWriter<int8_t> wr;
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            HGain<int8_t> gain(&wr, 2, 6);

            ASSERT_IS_EQUAL(gain.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_2, 6 * sizeof(int8_t)), 0);

            gain.SetGain(0.5f);
            ASSERT_IS_EQUAL(gain.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_0_5, 6 * sizeof(int8_t)), 0);

            gain.SetGain(-2);
            ASSERT_IS_EQUAL(gain.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected_neg2, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(gain.Write(input, 2), 2);

            try
            {
                gain.Write(input, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }
        }

        void test_gain_as_reader()
        {
            TestReader<int8_t> rd;
            HGain<int8_t> gain(&rd, 2, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(gain.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_2, 6 * sizeof(int8_t)), 0);

            gain.SetGain(0.5f);
            ASSERT_IS_EQUAL(gain.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_0_5, 6 * sizeof(int8_t)), 0);

            gain.SetGain(-2);
            ASSERT_IS_EQUAL(gain.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected_neg2, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(gain.Read(received, 2), 2);

            try
            {
                gain.Read(received, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }
        }
} hgain_test;