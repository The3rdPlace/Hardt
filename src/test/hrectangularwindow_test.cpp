#include "test.h"

class HRectangularWindow_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_short_apply);
            UNITTEST(test_long_apply);
            UNITTEST(test_apply);
        }

        const char* name()
        {
            return "HRectangularWindow";
        }

    private:

        void test_apply()
        {
            HRectangularWindow<int8_t> w;

            w.SetSize(6);

            int8_t input[6] = {1, 2, 3, 4, 5, 6};
            int8_t output[6];

            w.Apply(input, output, 6);
            ASSERT_IS_EQUAL(output[0], (int8_t) 1);
            ASSERT_IS_EQUAL(output[1], (int8_t) 2);
            ASSERT_IS_EQUAL(output[2], (int8_t) 3);
            ASSERT_IS_EQUAL(output[3], (int8_t) 4);
            ASSERT_IS_EQUAL(output[4], (int8_t) 5);
            ASSERT_IS_EQUAL(output[5], (int8_t) 6);
        }

        void test_short_apply()
        {
            HRectangularWindow<int8_t> w;

            w.SetSize(6);

            int8_t input[4] = {1, 2, 3, 4};
            int8_t output[4];

            try
            {
                w.Apply(input, output, 4);
                ASSERT_FAIL("Expected exception to be thrown, blocksize < 6");
            }
            catch(...)
            {}
        }

        void test_long_apply()
        {
            HRectangularWindow<int8_t> w;

            w.SetSize(6);

            int8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            int8_t output[8];

            try
            {
                w.Apply(input, output, 8);
                ASSERT_FAIL("Expected exception to be thrown, blocksize > 6");
            }
            catch(...)
            {}
        }

} hrectangularwindow_test;