#include <stdio.h>
#include <iostream>

#include "test.h"

class HFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_filter_as_writer);
            UNITTEST(test_filter_as_reader);
            UNITTEST(test_filter_with_probe);
        }

        const char* name()
        {
            return "HFilter";
        }

    private:

        int8_t expected[6] = {2, 4, 8, 16, 32, 64};
        
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

                int8_t output[8] = {1, 2, 4, 8, 16, 32, 0, 0};

            public:

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, output, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestFilter : public HFilter<T>
        {
            private:

                void Filter(T* src, T* dest, size_t blocksize)
                {
                    for( int i = 0; i < blocksize; i++ )
                    {
                        dest[i] = src[i] << 1;
                    }
                }

            public:

                TestFilter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL):
                    HFilter<T>(writer, blocksize, probe)
                {}

                TestFilter(HReader<T>* reader, size_t blocksize, HProbe<T>* probe = NULL):
                    HFilter<T>(reader, blocksize, probe)
                {}
        };

        void test_filter_as_writer()
        {
            TestWriter<int8_t> wr;
            TestFilter<int8_t> filter(&wr, 6);

            int8_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Write(input, 2), 2);

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
            TestReader<int8_t> rd;
            TestFilter<int8_t> filter(&rd, 6);

            int8_t received[6];
            ASSERT_IS_EQUAL(filter.Read(received, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 6 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(filter.Read(received, 2), 2);

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

        void test_filter_with_probe()
        {
            TestWriter<int8_t> wr;
            TestFilter<int8_t> filter(&wr, 6);
            HProbe<int8_t> probe("hfilter_test", true);

            int8_t input[8] = {1, 2, 4, 8, 16, 32, 0, 0};
            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            ASSERT_IS_EQUAL(memcmp((void*) wr.received, (void*) expected, 6 * sizeof(int8_t)), 0);

            std::ifstream resultfile("PROBE_hfilter_test.pcm", std::ios::in | std::ios::binary | std::ios::ate);
            if( !resultfile.is_open() ) {
                ASSERT_FAIL("No probe file found");
            }
        }

} hfilter_test;