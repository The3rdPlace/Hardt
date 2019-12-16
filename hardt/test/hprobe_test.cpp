#include <stdio.h>
#include <iostream>

#include "test.h"

class HProbe_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_probe_enabled);
            UNITTEST(test_probe_disabled);
        }

        const char* name()
        {
            return "HProbe";
        }

    private:

        void test_probe_enabled()
        {
            std::remove("PROBE_hprobe_enabled_test.pcm");

            int8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int8_t);
            HProbe<int8_t>* probe = new HProbe<int8_t>("hprobe_enabled_test", true);
            ASSERT_IS_EQUAL(probe->Write(data, datalen), datalen);
            delete probe;

            std::ifstream resultfile("PROBE_hprobe_enabled_test.pcm", std::ios::in | std::ios::binary | std::ios::ate);
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
                ASSERT_FAIL("File not found 'PROBE_hprobe_test.pcm'");
            }
        }

        void test_probe_disabled()
        {
            std::remove("PROBE_hprobe_disabled_test.pcm");

            int8_t data[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int datalen = sizeof(data) / sizeof(int8_t);
            HProbe<int8_t>* probe = new HProbe<int8_t>("hprobe_disabled_test", false);
            ASSERT_IS_EQUAL(probe->Write(data, datalen), datalen);
            delete probe;

            std::ifstream resultfile("PROBE_hprobe_disabled_test.pcm", std::ios::in | std::ios::binary | std::ios::ate);
            if (resultfile.is_open())
            {
                ASSERT_FAIL("File 'PROBE_hprobe_disabled_test.pcm' exists but the probe is disabled");

            }
        }

} hprobe_test;
