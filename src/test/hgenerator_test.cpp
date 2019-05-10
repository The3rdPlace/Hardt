#include <stdio.h>
#include <iostream>

#include "test.h"

class HGenerator_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_blocksize_100);
        }

        const char* name()
        {
            return "HGenerator";
        }

    private:

        class test_generator : public HGenerator<int8_t>
        {
            public:

                test_generator(int rate, int frequency):
                    HGenerator<int8_t>((H_SAMPLE_RATE) rate, frequency, 0)
                {
                }

                bool read(int8_t* dest, size_t size)
                {
                    GetSamples(dest, size);
                    return true;
                }
        };

        void test_blocksize_100()
        {
            int rate = 20;
            int frequency = 1;
            //test_generator tg(rate, frequency, 20);
            test_generator tg(100, 10);

            int8_t buffer[11];
            tg.read(buffer, 11);

            ASSERT_IS_EQUAL((int) buffer[0], 0);
            ASSERT_IS_EQUAL((int) buffer[1], 74);
            ASSERT_IS_EQUAL((int) buffer[2], 120);
            ASSERT_IS_EQUAL((int) buffer[3], 120);
            ASSERT_IS_EQUAL((int) buffer[4], 74);
            ASSERT_IS_EQUAL((int) buffer[5], 0);
            ASSERT_IS_EQUAL((int) buffer[6], -74);
            ASSERT_IS_EQUAL((int) buffer[7], -120);
            ASSERT_IS_EQUAL((int) buffer[8], -120);
            ASSERT_IS_EQUAL((int) buffer[9], -74);
            ASSERT_IS_EQUAL((int) buffer[10], 0);

            /*int samplesPerCycle = rate / frequency;
            int8_t signal[samplesPerCycle];
            int8_t expected[samplesPerCycle];

            int amplitude = 1;
            for ( int i = 0; i < samplesPerCycle; i++ )
            {
                expected[i] = (amplitude * sin((2 * M_PI * frequency * i) / (float) rate));
            }*/

            /*ASSERT_IS_TRUE(tg.read(signal, 20));
            for ( int i = 0; i < samplesPerCycle; i++ )
            {
                ASSERT_IS_EQUAL(expected[i], signal[i]);
            }*/
        }
} hgenerator_test;