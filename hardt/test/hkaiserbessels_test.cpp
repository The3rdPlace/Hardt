#include <stdio.h>
#include <iostream>

#include "test.h"

class HKaiserBessels_Test: public Test
{
    public:

        void run()
        {
            // Since the HKaiserBessel filter is merely a thin wrapper on
            // top of the HFirFilter, we only test the various Kaiser-Bessel
            // filters as writer. Reading and using the consumer pattern is
            // tested thoroughly by the FirFilter tests etc. We also do not
            // need to test command passing as this is also tested elsewhere

            UNITTEST(test_lowpass);
            UNITTEST(test_highpass);
            UNITTEST(test_bandpass);
            UNITTEST(test_bandstop);
        }

        const char* name()
        {
            return "HKaiserBessel(s)";
        }

    private:

        void test_lowpass()
        {
            /*
            int16_t input[6] = {1, 2, 3, 4, 5, 6};
            int16_t expected[6] = {0, 0, 0, 0, 0, 0};

            TestWriter<int16_t> wr(6);
            HFirFilter<int16_t> filter(wr.Writer(), HLowpassKaiserBessel( ... ).Calculate() );

            ASSERT_IS_EQUAL(filter.Write(input, 6), 6);
            for( int i = 0; i < 6; i++ ) {
                std::cout << "received[" << i << "] = " << std::to_string(wr.Received[i]) << std::endl;
            }
            //ASSERT_IS_EQUAL(memcmp((void*) wr.Received, (void*) expected, 6), 0);
            */
        }

        void test_highpass()
        {
        }

        void test_bandpass()
        {
        }

        void test_bandstop()
        {
        }

} hkaiserbessels_test;