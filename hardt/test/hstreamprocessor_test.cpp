#include <stdio.h>
#include <iostream>

#include "test.h"

class HStreamProcessor_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_read_write);
        }

        const char* name()
        {
            return "HStreamProcessor";
        }

    private:

        class TestReader : public HReader<int8_t>
        {
            private:

                int8_t* _data;

            public:

                TestReader(int8_t* data):
                    _data(data)
                {}

                int Read(int8_t* dest, size_t blocksize)
                {
                    memcpy(dest, _data, blocksize * sizeof(int8_t));
                    return blocksize;
                }
        };

        class TestWriter : public HWriter<int8_t>
        {
            private:

                int8_t* _received;
                bool* _terminated;

            public:

                TestWriter(int8_t* received, bool* terminationToken):
                    _received(received),
                    _terminated(terminationToken)
                {}

                int Write(int8_t* src, size_t blocksize)
                {
                    memcpy(_received, src, blocksize * sizeof(int8_t));
                    *_terminated = true;
                    return blocksize;
                }
        };

        void test_read_write()
        {
            bool terminated = false;
            int8_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int8_t received[14];
            memset(received, 0, 14 * sizeof(int8_t));

            TestReader rdr(expected);
            TestWriter wr(received, &terminated);
            HStreamProcessor<int8_t> proc(&wr, &rdr, 14, &terminated);

            proc.Run();

            ASSERT_IS_EQUAL(memcmp(received, expected, 14 * sizeof(int8_t)), 0);
        }
} hstreamprocessor_test;
