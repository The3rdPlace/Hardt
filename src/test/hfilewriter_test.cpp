#include <stdio.h>
#include <iostream>

#include "test.h"

class HFileWriter_Test: public Test
{
    public:

        void run()
        {
            test_int8();
            test_uint8();
            test_int16();
        }

        const char* name()
        {
            return "HFileWriter";
        }

    private:

        void test_int8()
        {
            int8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int8_t);
            HFileWriter<int8_t> *wr = new HFileWriter<int8_t>("/tmp/hfilewriter_int8_data.txt");
            wr->Start(NULL);
            ASSERT_IS_EQUAL(wr->Write(data, datalen), datalen);
            wr->Stop();
            delete wr;

            std::ifstream resultfile("/tmp/hfilewriter_int8_data.txt", std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                int size = resultfile.tellg();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(int8_t)));

                char* result = new char [size];
                resultfile.seekg (0, std::ios::beg);
                resultfile.read (result, size);
                resultfile.close();
                ASSERT_IS_EQUAL(memcmp(result, (char*) data, size), 0);

                delete[] result;
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilewriter_int8_data.txt'");
            }
        }

        void test_uint8()
        {
            uint8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(uint8_t);
            HFileWriter<uint8_t> *wr = new HFileWriter<uint8_t>("/tmp/hfilewriter_uint8_data.txt");
            wr->Start(NULL);
            ASSERT_IS_EQUAL(wr->Write(data, datalen), datalen);
            wr->Stop();
            delete wr;

            std::ifstream resultfile("/tmp/hfilewriter_uint8_data.txt", std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                int size = resultfile.tellg();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(uint8_t)));

                char* result = new char [size];
                resultfile.seekg (0, std::ios::beg);
                resultfile.read (result, size);
                resultfile.close();
                ASSERT_IS_EQUAL(memcmp(result, (char*) data, size), 0);

                delete[] result;
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilewriter_uint8_data.txt'");
            }
        }

        void test_int16()
        {
            int16_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int16_t);
            HFileWriter<int16_t> *wr = new HFileWriter<int16_t>("/tmp/hfilewriter_int_data.txt");
            wr->Start(NULL);
            ASSERT_IS_EQUAL(wr->Write(data, datalen), datalen);
            wr->Stop();
            delete wr;

            /* Do note, that the above integer array is stored lsb first on pc/linux systems. If
               you wish to exchange data with systems using another endianness, you must convert the
               byte order as appropriate */

            std::ifstream resultfile("/tmp/hfilewriter_int_data.txt", std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                int size = resultfile.tellg();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(int16_t)));

                char* result = new char [size];
                resultfile.seekg (0, std::ios::beg);
                resultfile.read (result, size);
                resultfile.close();
                ASSERT_IS_EQUAL(memcmp(result, (char*) data, size), 0);

                delete[] result;
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilewriter_int_data.txt'");
            }
        }
} hfilewriter_test;
