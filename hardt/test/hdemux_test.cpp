#include <stdio.h>
#include <iostream>

#include "test.h"

class HDeMux_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_demux);
        }

        const char* name()
        {
            return "HDeMux";
        }

    private:

        int8_t expected_1[8] = {1, 3, 5, 7, 0, 0, 0, 0};
        int8_t expected_2[8] = {2, 4, 6, 8, 0, 0, 0, 0};

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int8_t received[8];

                TestWriter()
                {
                    memset((void*) received, 0, 8 * sizeof(int8_t));
                }

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) received, src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_demux()
        {
            TestWriter<int8_t> wr_1;
            TestWriter<int8_t> wr_2;
            std::vector< HWriter<int8_t>* > writers = { &wr_1, &wr_2 };
            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            HDeMux<int8_t> demux(writers, 8);

            ASSERT_IS_EQUAL(demux.Write(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr_1.received, (void*) expected_1, 8 * sizeof(int8_t)), 0);
            ASSERT_IS_EQUAL(memcmp((void*) wr_2.received, (void*) expected_2, 8 * sizeof(int8_t)), 0);
        }
} hdemux_test;