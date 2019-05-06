#include <stdio.h>
#include <iostream>

#include "test.h"

class HFileReader_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_int8);
            UNITTEST(test_uint8);
            UNITTEST(test_int16);
        }

        const char* name()
        {
            return "HFileReader";
        }

    private:

        void test_int8()
        {
            int8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int8_t);

            std::ofstream resultfile("/tmp/hfilereader_int8_data.txt", std::ios::binary);
            if( resultfile.is_open() )
            {
                resultfile.write((char*) data, datalen * sizeof(int8_t));
                int size = resultfile.tellp();
                resultfile.close();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(int8_t)));
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilereader_int8_data.txt'");
            }


            int8_t result[datalen];
            HFileReader<int8_t> *rd = new HFileReader<int8_t>("/tmp/hfilereader_int8_data.txt");
            rd->Start(NULL);
            ASSERT_IS_EQUAL(rd->Read(result, datalen), datalen);
            rd->Stop();
            delete rd;
            ASSERT_IS_EQUAL(memcmp((char*) result, (char*) data, datalen * sizeof(int8_t)), 0);
        }

        void test_uint8()
        {
            uint8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(uint8_t);


            std::ofstream resultfile("/tmp/hfilereader_uint8_data.txt", std::ios::binary);
            if( resultfile.is_open() )
            {
                resultfile.write((char*) data, datalen * sizeof(uint8_t));
                int size = resultfile.tellp();
                resultfile.close();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(uint8_t)));
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilereader_uint8_data.txt'");
            }


            uint8_t result[datalen];
            HFileReader<uint8_t> *rd = new HFileReader<uint8_t>("/tmp/hfilereader_uint8_data.txt");
            rd->Start(NULL);
            ASSERT_IS_EQUAL(rd->Read(result, datalen), datalen);
            rd->Stop();
            delete rd;
            ASSERT_IS_EQUAL(memcmp((char*) result, (char*) data, datalen * sizeof(uint8_t)), 0);
        }

        void test_int16()
        {
            int16_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int16_t);

            std::ofstream resultfile("/tmp/hfilereader_int16_data.txt", std::ios::binary);
            if( resultfile.is_open() )
            {
                resultfile.write((char*) data, datalen * sizeof(int16_t));
                int size = resultfile.tellp();
                resultfile.close();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(int16_t)));
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilereader_int16_data.txt'");
            }


            int16_t result[datalen];
            HFileReader<int16_t> *rd = new HFileReader<int16_t>("/tmp/hfilereader_int16_data.txt");
            rd->Start(NULL);
            ASSERT_IS_EQUAL(rd->Read(result, datalen), datalen);
            rd->Stop();
            delete rd;
            ASSERT_IS_EQUAL(memcmp((char*) result, (char*) data, datalen * sizeof(int16_t)), 0);
        }
} hfilereader_test;
