#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "test.h"

class HWavReader_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_int16_1_channel);
        }

        const char* name()
        {
            return "HWavReader";
        }

    private:

        void test_int16_1_channel()
        {
            // Todo: use relative path, possibly by injecting root or resource directory from cmake
            HWavReader<int16_t>* rd = new HWavReader<int16_t>("/Users/henrik/Git/Hardt/src/test/resources/16bit_48k_mono.wav");
            ASSERT_IS_TRUE(rd->Start(NULL));
            ASSERT_IS_EQUAL(rd->GetFormat(), H_SAMPLE_FORMAT_INT_16);
            ASSERT_IS_EQUAL(rd->GetRate(), 48000);
            ASSERT_IS_EQUAL(rd->GetSize(), (size_t) 338760);
            ASSERT_IS_EQUAL(rd->GetChannels(), 1);

            int16_t buffer[2048];
            ASSERT_IS_EQUAL(rd->Read(buffer, 2048 / sizeof(int16_t)), (int) (2048 / sizeof(int16_t)));

            rd->Stop();
            delete rd;
        }
} hwavreader_test;
