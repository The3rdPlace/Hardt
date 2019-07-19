#include <stdio.h>
#include <iostream>

#include "test.h"

class HIirFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);
        }

        const char* name()
        {
            return "HIirFilter";
        }

    private:

        /*
            First write:

                Initial taps buffer: 0 0 0
                Initial output buffer: 0 0 0

                1 0 0 = 1 0 0 + 0 0 = 1

                    taps = 1 0 0 => 0 1 0
                    output = 1 0 0 => 0 1 0

                2 1 0 = 2 2 0 - 1 0 = 3

                    taps = 2 1 0 => 0 2 1
                    output = 3 1 0 => 0 3 1

                4 2 1 = 4 4 3 - 3 2 = 6

                    taps = 4 2 1 => 0 4 2
                    output = 6 3 1 => 0 6 3

                8 4 2 = 8 8 6 - 6 6 = 10

                    taps = 8 4 2 => 0 8 4
                    output = 10 6 3 => 0 10 6

                16 8 4 = 16 16 12 - 10 12 = 22

                    taps = 16 8 4 => 0 16 8
                    output = 22 10 6 => 0 22 10

                32 16 8 = 32 32 24 - 22 20 = 46

                Expected = 1 3 6 10 22 46

            Second write:

                Initial taps buffer: 32 16 8
                Initial output buffer: 46 22 10

                    taps = 32 16 8 => 0 32 16
                    output = 46 22 10 => 0 46 22

                1 32 16 = 1 64 48 - 46 44 = 23

                    taps = 1 32 16 => 0 1 32
                    output = 23 46 22 => 0 23 46

                2 1 32 = 2 2 96 - 23 92 = -15

                Expected = 23 -15
         */

        // b and a coefficients are reversed in the list - most designers return the list
        // of coefficients this way!
        float coeefs[5] = { 1.0, 2.0, 3.0, 1.0, 2.0 };
        int16_t expected[6] = {1, 3, 6, 10, 22, 46};
        int16_t expectedNext[2] = {23, -15};

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int16_t received[6];

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

                int16_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};

            public:

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, output, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_filter_as_writer()
        {
            TestWriter<int16_t> wr;
            HIirFilter<int16_t> filter(&wr, coeefs, 5, 6);

            int16_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected, 6 * sizeof(int16_t)), 0);

            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

            try
            {
                filter.Write(input, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }
        }

        void test_filter_as_reader()
        {
            TestReader<int16_t> rd;
            HIirFilter<int16_t> filter(&rd, coeefs, 5, 6);

            int16_t received[6];
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int16_t)), 0);

            ASSERT_IS_EQUAL(filter.Read(received, 2), 2);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 2 * sizeof(int16_t)), 0);

            try
            {
                filter.Read(received, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }
        }
} HIirFilter_test;