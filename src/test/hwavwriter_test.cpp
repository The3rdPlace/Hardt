#include <stdio.h>
#include <iostream>

#include "test.h"

class HWavWriter_Test: public Test
{
    private:

        uint8_t _data[2048];

    public:

        HWavWriter_Test()
        {
            // Build test data
            uint8_t pattern[] = {0x01, 0x02, 0x03, 0x04};
            for( int pos = 0; pos < 2048; pos += 4)
            {
                memcpy((void*) &_data[pos], (void*) pattern, 4);
            }
        }

        void run()
        {
            UNITTEST(test_uint8_1_channel);
            UNITTEST(test_int16_2_channels);
        }

        const char* name()
        {
            return "HWavWriter";
        }

    private:

        void CheckResult(const char* filename, char* expected, int expectedSize)
        {
            // Read the file and compare with the expected data
            std::ifstream resultfile(filename, std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                int size = resultfile.tellg();
                ASSERT_IS_EQUAL(size, expectedSize);

                uint8_t* result = new uint8_t[size];
                resultfile.seekg (0, std::ios::beg);
                resultfile.read ((char*) result, size);
                resultfile.close();
                ASSERT_IS_EQUAL(memcmp(result, expected, expectedSize), 0);

                delete[] result;
            }
            else
            {
                ASSERT_FAIL("File not found");
            }
        }

        void test_uint8_1_channel()
        {
            uint8_t header[] = {
                0x52, // 'R'
                0x49, // 'I'
                0x46, // 'F'
                0x46, // 'F'
                0x24, // Size of data = 44 + 2048 - 8 = 2084 = 00 00 08 24
                0x08, // ..
                0x00, // ..
                0x00, // ..
                0x57, // 'W'
                0x41, // 'A'
                0x56, // 'V'
                0x45, // 'E'

                0x66, // 'f'
                0x6d, // 'm'
                0x74, // 't'
                0x20, // (blankspace)
                0x10, // Chunksize for 'fmt' chunk. Always 16 bytes for wav
                0x00, // ..
                0x00, // ..
                0x00, // ..
                0x01, // Audio format = 1 (PCM)
                0x00, // ..
                0x01, // Number of channels = 1
                0x00, // ..
                0x40, // Samplerate = 8000 = 00 00 1F 40
                0x1f, // ..
                0x00, // ..
                0x00, // ..
                0x40, // Byterate = samplerate * channels * sizeof(int18_t) = 8000 * 1 * 1 = 8000 = 00 00 1F 40
                0x1f, // ..
                0x00, // ..
                0x00, // ..
                0x01, // Blockalign = channels * sizeof(int16_t) = 1 * 1 = 1
                0x00, // ..
                0x08, // Bits per sample = sizeof(int18_t) = 8
                0x00, // ..

                0x64, // 'd'
                0x61, // 'a'
                0x74, // 't'
                0x61, // 'a'
                0x00, // Total size of data = 2048 2048 samples)
                0x08, // ..
                0x00, // ..
                0x00, // ..
            };

            uint8_t expected[44 + 2048];
            memcpy((void*) expected, (void*) header, 44);
            memcpy((void*) &expected[44], (void*) _data, 2048);

            // Create writer and write the wav file
            HWavWriter<uint8_t>* wr = new HWavWriter<uint8_t>("/tmp/hwavwriter_uint8_data.txt", H_SAMPLE_FORMAT_UINT_8, 1, H_SAMPLE_RATE_8K);
            wr->Start(NULL);
            wr->Write((uint8_t*) _data, 2048 / sizeof(uint8_t));
            wr->Stop();
            delete wr;

            CheckResult("/tmp/hwavwriter_uint8_data.txt", (char*) expected, 44 + 2048);
        }

        void test_int16_2_channels()
        {
            uint8_t header[] = {
                0x52, // 'R'
                0x49, // 'I'
                0x46, // 'F'
                0x46, // 'F'
                0x24, // Size of data = 44 + 2048 - 8 = 2084 = 00 00 08 24
                0x08, // ..
                0x00, // ..
                0x00, // ..
                0x57, // 'W'
                0x41, // 'A'
                0x56, // 'V'
                0x45, // 'E'

                0x66, // 'f'
                0x6d, // 'm'
                0x74, // 't'
                0x20, // (blankspace)
                0x10, // Chunksize for 'fmt' chunk. Always 16 bytes for wav
                0x00, // ..
                0x00, // ..
                0x00, // ..
                0x01, // Audio format = 1 (PCM)
                0x00, // ..
                0x02, // Number of channels = 2
                0x00, // ..
                0x22, // Samplerate = 22050 = 00 00 56 22
                0x56, // ..
                0x00, // ..
                0x00, // ..
                0x88, // Byterate = samplerate * channels * sizeof(int16_t) = 22050 * 2 * 2 = 88200 = 00 01 58 88
                0x58, // ..
                0x01, // ..
                0x00, // ..
                0x04, // Blockalign = channels * sizeof(int16_t) = 2 * 2 = 4
                0x00, // ..
                0x10, // Bits per sample = sizeof(int16_t) = 16
                0x00, // ..

                0x64, // 'd'
                0x61, // 'a'
                0x74, // 't'
                0x61, // 'a'
                0x00, // Total size of data = 2048 (512 samples)
                0x08, // ..
                0x00, // ..
                0x00, // ..
            };


            uint8_t expected[44 + 2048];
            memcpy((void*) expected, (void*) header, 44);
            memcpy((void*) &expected[44], (void*) _data, 2048);

            // Create writer and write the wav file
            HWavWriter<int16_t>* wr = new HWavWriter<int16_t>("/tmp/hwavwriter_uint16_data.txt", H_SAMPLE_FORMAT_INT_16, 2, H_SAMPLE_RATE_22K);
            wr->Start(NULL);
            wr->Write((int16_t*) _data, 2048 / sizeof(int16_t));
            wr->Stop();
            delete wr;

            CheckResult("/tmp/hwavwriter_uint16_data.txt", (char*) expected, 44 + 2048);
        }
} hwavwriter_test;
