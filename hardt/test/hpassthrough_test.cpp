#include <stdio.h>
#include <iostream>

#include "test.h"

class HPassThrough_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_passthrough_as_writer);
            UNITTEST(test_passthrough_as_reader);
        }

        const char* name()
        {
            return "HPassThrough";
        }

    private:

        int8_t in_out[6] = {2, 4, 6, 8, 10, 12};

        template <class T>
        class TestReader : public HReader<T>
        {
            private:

                int8_t in_out[6] = {2, 4, 6, 8, 10, 12};

            public:

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, in_out, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_passthrough_as_writer()
        {
            TestWriter2<int8_t> wr(6);
            HPassThrough<int8_t> passthrough(wr.Writer(), 6);

            ASSERT_IS_EQUAL(passthrough.Write(in_out, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) in_out, 6 * sizeof(int8_t)), 0);

            try
            {
                passthrough.Write(in_out, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }
        }

        void test_passthrough_as_reader()
        {
            TestReader<int8_t> rd;
            HPassThrough<int8_t> passthrough(&rd, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(passthrough.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) in_out, 6 * sizeof(int8_t)), 0);

            try
            {
                passthrough.Read(received, 8);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }
        }
} hpassthrough_test;