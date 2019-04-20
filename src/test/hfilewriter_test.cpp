#include <stdio.h>
#include <iostream>

class HFileWriter_Test: public Test
{
    public:

        void run()
        {
            test_char();
            test_int();
        }

    private:

        void test_char()
        {
            char data[] = "123...abc...789...xyz";
            HFileWriter<char> *wr = new HFileWriter<char>("/tmp/hfilewriter_char_data.txt", strlen(data));
            ASSERT_IS_EQUAL(wr->Write(data), (int) strlen(data));
            delete wr;

            std::ifstream resultfile("/tmp/hfilewriter_char_data.txt", std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                int size = resultfile.tellg();
                ASSERT_IS_EQUAL(size, (int) strlen(data));

                char* result = new char [size];
                resultfile.seekg (0, std::ios::beg);
                resultfile.read (result, size);
                resultfile.close();
                ASSERT_IS_EQUAL(memcmp(result, data, strlen(data)), 0);

                delete[] result;
            }
            else
            {
                ASSERT_FAIL("File not found '/tmp/hfilewriter_char_data.txt'");
            }
        }

        void test_int()
        {
            int data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int);
            std::cout << "size " << datalen;
            HFileWriter<int> *wr = new HFileWriter<int>("/tmp/hfilewriter_int_data.txt", datalen);
            ASSERT_IS_EQUAL(wr->Write(data), datalen);
            delete wr;

            /* Do note, that the above integer array is stored lsb first on pc/linux systems. If
               you wish to exchange data with systems using another endianness, you must convert the
               byte order as appropriate */
               
            std::ifstream resultfile("/tmp/hfilewriter_int_data.txt", std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                int size = resultfile.tellg();
                ASSERT_IS_EQUAL(size, (int) (datalen * sizeof(int)));

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
