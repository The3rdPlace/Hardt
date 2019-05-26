#include "test.h"

class HNChunkWriter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_write);
            UNITTEST(test_write_chunked);
        }

        const char* name()
        {
            return "HChunkWriter";
        }

    private:

        class TestWriter : public HChunkWriter<int8_t>
        {
            public:

                int writes;
                int8_t received[1024];

                TestWriter():
                    HChunkWriter()
                {}
                
                TestWriter(int chunksize):
                    HChunkWriter(chunksize)
                {}

                int WriteChunk(int8_t* src, size_t chunksize)
                {
                    memcpy(&received[writes * GetChunksize()], src, chunksize);
                    writes++;
                    return chunksize;
                }
        };

        void test_write()
        {
            TestWriter wr;
            int8_t input[1024];
            memset(input, 0x42, 512);
            memset(&input[512], 0x24, 512);

            try
            {
                ASSERT_IS_EQUAL(wr.Write(input, 1000), 1000);
            }
            catch(...)
            {
                ASSERT_FAIL("Did not expect exception to be thrown, chunking is disabled");
            }

            try
            {
                ASSERT_IS_EQUAL(wr.Write(input, 500), 500);
            }
            catch(...)
            {
                ASSERT_FAIL("Did not expect exception to be thrown, chunking is disabled");
            }

            try
            {
                wr.writes = 0;
                memset(wr.received, 0, 1024);
                ASSERT_IS_EQUAL(wr.Write(input, 512), 512);
                ASSERT_IS_EQUAL(wr.writes, 1);
                ASSERT_IS_EQUAL((int) wr.received[0], 0x42);

                wr.writes = 0;
                memset(wr.received, 0, 1024);
                ASSERT_IS_EQUAL(wr.Write(input, 1024), 1024);
                ASSERT_IS_EQUAL(wr.writes, 1);
                ASSERT_IS_EQUAL((int) wr.received[0], 0x42);
                ASSERT_IS_EQUAL((int) wr.received[512], 0x24);
            }
            catch(...)
            {
                ASSERT_FAIL("Did not expect exception to be thrown, chunking is disabled");
            }
        }

        void test_write_chunked()
        {
            TestWriter wr(512);
            int8_t input[1024];
            memset(input, 0x42, 512);
            memset(&input[512], 0x24, 512);

            try
            {
                wr.Write(input, 1000); // 1000 / 512 = 1.953....
                ASSERT_FAIL("Expected exception to be thrown, number of chunks > 1 and < 2");
            }
            catch(...)
            {}

            try
            {
                wr.Write(input, 500); // 500 / 512 = 0.976....
                ASSERT_FAIL("Expected exception to be thrown, number of chunks < 1");
            }
            catch(...)
            {}

            try
            {
                wr.writes = 0;
                memset(wr.received, 0, 1024);
                ASSERT_IS_EQUAL(wr.Write(input, 512), 512);
                ASSERT_IS_EQUAL(wr.writes, 1);
                ASSERT_IS_EQUAL((int) wr.received[0], 0x42);

                wr.writes = 0;
                memset(wr.received, 0, 1024);
                ASSERT_IS_EQUAL(wr.Write(input, 1024), 1024);
                ASSERT_IS_EQUAL(wr.writes, 2);
                ASSERT_IS_EQUAL((int) wr.received[0], 0x42);
                ASSERT_IS_EQUAL((int) wr.received[512], 0x24);
            }
            catch(...)
            {
                ASSERT_FAIL("Did not expect exception to be thrown, number of chunks = 1 and 2");
            }
        }

} hchunkwriter_test;