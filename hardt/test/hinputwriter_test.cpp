#include <stdio.h>
#include <iostream>

#include "test.h"

class HInputWriter_Test: public Test
{
public:

    void run()
    {
        UNITTEST(test_writer);
        UNITTEST(test_writer_autostart);
    }

    const char* name()
    {
        return "HInputWriter";
    }

private:

    void test_writer()
    {
        TestWriter<int8_t> wr(8);
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        HInputWriter<int8_t> iw(wr.Writer(), false);

        ASSERT_IS_EQUAL(iw.Write(input, 8), 8);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

        ASSERT_IS_EQUAL(wr.Started, 0);

        ASSERT_IS_TRUE(iw.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

    void test_writer_autostart()
    {
        TestWriter<int8_t> wr(8);
        int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        int8_t expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        HInputWriter<int8_t> iw(wr.Writer());

        ASSERT_IS_EQUAL(iw.Write(input, 8), 8);
        ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 8 * sizeof(int8_t)), 0);

        ASSERT_IS_EQUAL(wr.Started, 1);

        ASSERT_IS_TRUE(iw.Command(&TestNopCommand));
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

} hinputwriter_test;