#include <stdio.h>
#include <iostream>

#include "test.h"

class HIirFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_filter_as_writer);
            //UNITTEST(test_filter_as_reader);
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

                2 1 0 = 2 2 0 + 1 0 = 5

                    taps = 2 1 0 => 0 2 1
                    output = 5 1 0 => 0 5 1

                4 2 1 = 4 4 3 + 5 2 = 18

                    taps = 4 2 1 => 0 4 2
                    output = 18 5 1 => 0 18 5

                8 4 2 = 8 8 6 + 18 10 = 50

                    taps = 8 4 2 => 0 8 4
                    output = 50 18 5 => 0 50 18

                16 8 4 = 16 16 12 + 50 36 = 130

                    taps = 16 8 4 => 0 16 8
                    output = 130 50 18 => 0 130 50

                32 16 8 = 32 32 24 +  130 100 = 318

                Expected = 1 5 18 50 130 318

            Second write:

                Initial taps buffer: 32 16 8
                Initial output buffer: 318 130 50

                    taps = 32 16 8 => 0 32 16
                    output = 318 130 50 => 0 318 130

                1 32 16 = 1 64 48 + 318 260 = 691

                    taps = 1 32 16 => 0 1 32
                    output = 691 318 130 => 0 691 318

                2 1 32 = 2 2 96 + 691 636 = 1427

                Expected = 691 1427
         */
        float coeefs[5] = { 1.0, 2.0, 1.0, 2.0, 3.0 };
        int16_t expected[6] = {1, 5, 18, 50, 130, 318};
        int16_t expectedNext[2] = {691, 1427};

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