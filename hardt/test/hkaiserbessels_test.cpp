#include <stdio.h>
#include <iostream>

#include "test.h"

class HKaiserBessels_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_lowpass_low);
            UNITTEST(test_lowpass_med);
            UNITTEST(test_lowpass_high);
            UNITTEST(test_highpass_low);
            UNITTEST(test_highpass_med);
            UNITTEST(test_highpass_high);
            UNITTEST(test_bandpass_low);
            UNITTEST(test_bandpass_med);
            UNITTEST(test_bandpass_high);
            UNITTEST(test_bandstop_low);
            UNITTEST(test_bandstop_med);
            UNITTEST(test_bandstop_high);
        }

        const char* name()
        {
            return "HKaiserBessel(s)";
        }

    private:

        void test_lowpass_low()
        {
            float expected[15] = {-0.045125, -0.037555, -0.008351, 0.039747, 0.097985, 0.153690, 0.193733, 0.208292, 0.193733, 0.153690, 0.097985, 0.039747, -0.008351, -0.037555, -0.045125};

            HLowpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 20);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_lowpass_med()
        {
            float expected[15] = {-0.002506, -0.037555, -0.008351, 0.039747, 0.097985, 0.153690, 0.193733, 0.208292, 0.193733, 0.153690, 0.097985, 0.039747, -0.008351, -0.037555, -0.002506};

            HLowpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 50);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_lowpass_high()
        {
            float expected[15] = {-0.000920, -0.037555, -0.008351, 0.039747, 0.097985, 0.153690, 0.193733, 0.208292, 0.193733, 0.153690, 0.097985, 0.039747, -0.008351, -0.037555, -0.000920};

            HLowpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 60);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_highpass_low()
        {
            float expected[15] = {0.045084, 0.037513, 0.008310, -0.039789, -0.098027, -0.153732, -0.193775, 0.791667, -0.193775, -0.153732, -0.098027, -0.039789, 0.008310, 0.037513, 0.045084};

            HHighpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 20);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_highpass_med()
        {
            float expected[15] = {0.002504, 0.037513, 0.008310, -0.039789, -0.098027, -0.153732, -0.193775, 0.791667, -0.193775, -0.153732, -0.098027, -0.039789, 0.008310, 0.037513, 0.002504};

            HHighpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 50);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_highpass_high()
        {
            float expected[15] = {0.000919, 0.037513, 0.008310, -0.039789, -0.098027, -0.153732, -0.193775, 0.791667, -0.193775, -0.153732, -0.098027, -0.039789, 0.008310, 0.037513, 0.000919};

            HHighpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 60);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_bandpass_low()
        {
            float expected[15] = {0.056853, 0.090565, 0.024786, -0.108705, -0.173053, -0.074154, 0.113689, 0.208333, 0.113689, -0.074154, -0.173053, -0.108705, 0.024786, 0.090565, 0.056853};

            HBandpassKaiserBessel<int16_t> kb(5000, 10000, H_SAMPLE_RATE_48K, 15, 20);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_bandpass_med()
        {
            float expected[15] = {0.003158, 0.090565, 0.024786, -0.108705, -0.173053, -0.074154, 0.113689, 0.208333, 0.113689, -0.074154, -0.173053, -0.108705, 0.024786, 0.090565, 0.003158};

            HBandpassKaiserBessel<int16_t> kb(5000, 10000, H_SAMPLE_RATE_48K, 15, 50);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_bandpass_high()
        {
            float expected[15] = {0.001159, 0.090565, 0.024786, -0.108705, -0.173053, -0.074154, 0.113689, 0.208333, 0.113689, -0.074154, -0.173053, -0.108705, 0.024786, 0.090565, 0.001159};

            HBandpassKaiserBessel<int16_t> kb(5000, 10000, H_SAMPLE_RATE_48K, 15, 60);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_bandstop_low()
        {
            float expected[15] = {0.045084, 0.037513, 0.008310, -0.039789, -0.098027, -0.153732, -0.193775, 0.791667, -0.193775, -0.153732, -0.098027, -0.039789, 0.008310, 0.037513, 0.045084};

            HHighpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 20);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_bandstop_med()
        {
            float expected[15] = {0.002504, 0.037513, 0.008310, -0.039789, -0.098027, -0.153732, -0.193775, 0.791667, -0.193775, -0.153732, -0.098027, -0.039789, 0.008310, 0.037513, 0.002504};

            HHighpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 50);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }

        void test_bandstop_high()
        {
            float expected[15] = {0.000919, 0.037513, 0.008310, -0.039789, -0.098027, -0.153732, -0.193775, 0.791667, -0.193775, -0.153732, -0.098027, -0.039789, 0.008310, 0.037513, 0.000919};

            HHighpassKaiserBessel<int16_t> kb(5000, H_SAMPLE_RATE_48K, 15, 60);
            float* coefficients = kb.Calculate();

            for( int i = 0; i < 15; i++ ) {
                if( round( coefficients[i] * 1000000 ) != round( expected[i] * 1000000 ) ) {
                    ASSERT_FAIL("Coefficients differ: " + std::to_string(coefficients[i]) + " != " + std::to_string(expected[i]));
                }
            }
        }


} hkaiserbessels_test;