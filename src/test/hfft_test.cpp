#include <stdio.h>
#include <iostream>

#include "test.h"

class HFft_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_fft_static_callback);
            UNITTEST(test_fft_static_callback_with_factory);
            UNITTEST(test_fft_member_callback);
        }

        const char* name()
        {
            return "HFft";
        }

    private:

        void writeToFFT(HFft<int8_t>* fft)
        {
            // Todo: Add proper test data
            int8_t input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
            ASSERT_IS_EQUAL(fft->Write(input, 10), 10);
        }

        static void verifyFFT(long int* data, size_t length)
        {
            // Todo: Add test of resulting fft
        }

        static void staticCallback(long int* data, size_t length)
        {
            verifyFFT(data,length);
        }

        void memberCallback(long int* data, size_t length)
        {
            verifyFFT(data,length);
        }

        void test_fft_static_callback()
        {
            auto fft = HFft<int8_t>(10, 5, 1, staticCallback);
            writeToFFT(&fft);
        }

        void test_fft_static_callback_with_factory()
        {
            auto fft = HFft<int8_t>::Create(10, 5, 1, staticCallback);
            writeToFFT(fft);
            delete fft;
        }

        void test_fft_member_callback()
        {
            auto fft = HFft<int8_t>::Create<HFft_Test>(10, 5, 1, this, &HFft_Test::memberCallback);
            writeToFFT(fft);
            delete fft;
        }
} hfft_test;