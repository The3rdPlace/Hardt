#include "test.h"

class HWindow_Test: public Test
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
            return "HWindow";
        }

    private:

        class TestWindow : public HWindow<int8_t>
        {
            public:

                float ValueAt(int N, int n)
                {
                    float factors[] = {0, 0.5, 1, 1, 0.5, 0};
                    return factors[n];
                }
        };

        void test_apply()
        {
            TestWindow w;

            w.SetSize(6);

            int8_t input[6] = {2, 2, 2, 2, 2, 2};
            int8_t output[6];

            w.Apply(input, output, 6);
            ASSERT_IS_EQUAL(output[0], (int8_t) 0);
            ASSERT_IS_EQUAL(output[1], (int8_t) 1);
            ASSERT_IS_EQUAL(output[2], (int8_t) 2);
            ASSERT_IS_EQUAL(output[3], (int8_t) 2);
            ASSERT_IS_EQUAL(output[4], (int8_t) 1);
            ASSERT_IS_EQUAL(output[5], (int8_t) 0);
        }

        void test_short_apply()
        {
            TestWindow w;

            w.SetSize(6);

            int8_t input[4] = {2, 2, 2, 2};
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
            TestWindow w;

            w.SetSize(6);

            int8_t input[8] = {2, 2, 2, 2, 2, 2, 2, 2};
            int8_t output[8];

            try
            {
                w.Apply(input, output, 8);
                ASSERT_FAIL("Expected exception to be thrown, blocksize > 6");
            }
            catch(...)
            {}
        }

} hwindow_test;