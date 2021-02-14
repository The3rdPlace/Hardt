#include <stdio.h>
#include <iostream>

#include "test.h"

class HPassthroughFileWriter_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_withWriter);
        }

        const char* name()
        {
            return "HPassthroughFileWriter";
        }

    private:

        void test_withWriter()
        {
            int8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            TestWriter<int8_t> outWriter;

            HPassthroughFileWriter<int8_t> *wr = new HPassthroughFileWriter<int8_t>(outWriter, 14, "/tmp/hfilewriter_int8_data.txt", false);
            wr->Start();

            ASSERT_IS_EQUAL(wr->Write(data, 14), datalen);

            ASSERT_IS_EQUAL(wr->Write(data, 14), datalen);


            wr->Stop();
            delete wr;

            /*std::ifstream resultfile("/tmp/hfilewriter_int8_data.txt", std::ios::in | std::ios::binary | std::ios::ate);
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
            }*/
        }
} hpassthroughfilewriter_test;
