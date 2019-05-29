#include "test.h"

#include <string.h>

class HHammingWindow_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_apply);
            UNITTEST(test_values);
        }

        const char* name()
        {
            return "HHammingWindow";
        }

    private:

        void test_apply()
        {
            HHammingWindow<int8_t> w;

            w.SetSize(6);

            int8_t input[6] = {10, 10, 10, 10, 10, 10};
            int8_t output[6];

            w.Apply(input, output, 6);
            for(int i = 0; i < 6; i++ )
            {
                int8_t expected = 1;
                std::cout << "expected " << (int) expected << ", got " << (int) output[i] << std::endl;
            }
            ASSERT_IS_EQUAL(output[0], (int8_t) 3);
            ASSERT_IS_EQUAL(output[1], (int8_t) 4);
            ASSERT_IS_EQUAL(output[2], (int8_t) 6);
            ASSERT_IS_EQUAL(output[3], (int8_t) 7);
            ASSERT_IS_EQUAL(output[4], (int8_t) 6);
            ASSERT_IS_EQUAL(output[5], (int8_t) 4);
        }

        void test_values_for_size(HWindow<int8_t>* w, int size)
        {
            w->SetSize(size);

            int8_t input[size];
            memset((void*) input, 1, size * sizeof(int8_t));

            int8_t output[size];
            w->Apply(input, output, size);

            for( int i = 0; i < size; i++)
            {
                ASSERT_IS_LESS_THAN_OR_EQUAL((int) output[i], 1);
                ASSERT_IS_GREATER_THAN_OR_EQUAL(output[i], (int8_t) 0);
            }
        }

        void test_values()
        {
            HHammingWindow<int8_t> w;

            test_values_for_size(&w, 10);
            test_values_for_size(&w, 128);
            test_values_for_size(&w, 512);
            test_values_for_size(&w, 1024);
            test_values_for_size(&w, 2048);
            test_values_for_size(&w, 4096);

        }
} hhammingwindow_test;