#include <stdio.h>
#include <iostream>

#include "test.h"

#include "hmovingaveragefilter.h"

class HMovingAverageFilter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_movingaveragefilter_as_writer);
            UNITTEST(test_movingaveragefilter_as_reader);
        }

        const char* name()
        {
            return "HMovingAverageFilter";
        }

    private:

        /*

            Moving average for M = 2:

                First write:

                    Previous: 0
                    Source: 2 4 3 4 10 6 4 2

                    I = 0

                        avg = (0 + 2) / 2 = 1

                    I = 1

                        avg = (2 + 4 ) / 2 = 3

                    I = 2

                        avg = (4 + 3) / 2 = 3

                    I = 3

                        avg = (3 + 4) / 2 = 3

                    I = 4

                        avg = (4 + 10) / 2 = 7

                    I = 5

                        avg = (10 + 6) / 2 = 8

                    I = 6

                        avg = (6 + 4) / 2 = 5

                    I = 7

                        avg = (4 + 2) / 2 = 3

                    Dest: 1 3 3 3 7 8 5 3
                    Previous: 2

                Second write:

                    Previous: 2
                    Source: 2 4 3 4 10 6 4 2

                    I = 0

                        avg = (2 + 2) / 2 = 2

                    I = 1

                        avg = (2 + 4 ) / 2 = 3

                    I = 2

                        avg = (4 + 3) / 2 = 3

                    I = 3

                        avg = (3 + 4) / 2 = 3

                    I = 4

                        avg = (4 + 0) / 2 = 2

                    I = 5

                        avg = (0 + 6) / 2 = 3

                    I = 6

                        avg = (6 + 4) / 2 = 5

                    I = 7

                        avg = (4 + 2) / 2 = 3

                    Dest: 2 3 3 3 2 3 5 3
                    Previous: 2

        */

        int8_t expected[8] = {1, 3, 3, 3, 7, 8, 5, 3};
        int8_t expectedNext[8] = {2, 3, 3, 3, 7, 8, 5, 3};

        void test_movingaveragefilter_as_writer()
        {
            TestWriter<int8_t> wr(8);
            int8_t input[8] = {2, 4, 3, 4, 10, 6, 4, 2};
            HMovingAverageFilter<int8_t> maf(wr.Writer(), 2, 8);

            ASSERT_IS_EQUAL(maf.Write(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(maf.Write(input, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expectedNext, 8 * sizeof(int8_t)), 0);

            try
            {
                maf.Write(input, 9);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(maf.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(wr.Commands, 1);
        }

        void test_movingaveragefilter_as_reader()
        {
            int8_t output[8] = {2, 4, 3, 4, 10, 6, 4, 2};
            TestReader<int8_t> rd(output, 8);
            HMovingAverageFilter<int8_t> maf(&rd, 2, 8);

            int8_t received[8];
            ASSERT_IS_EQUAL(maf.Read(received, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expected, 8 * sizeof(int8_t)), 0);

            ASSERT_IS_EQUAL(maf.Read(received, 8), 8);
            ASSERT_IS_EQUAL(memcmp((void*) received, (void*) expectedNext, 8 * sizeof(int8_t)), 0);

            try
            {
                maf.Read(received, 9);
                ASSERT_FAIL("Expected HFilterIOException");
            }
            catch(HFilterIOException*)
            {}
            catch( ... )
            {
                ASSERT_FAIL("Expected HFilterIOException, but got other type");
            }

            ASSERT_IS_TRUE(maf.Command(&TestNopCommand));
            ASSERT_IS_EQUAL(rd.Commands, 1);
        }
} hmovingaveragefilter_test;