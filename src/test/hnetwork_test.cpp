#include <stdio.h>
#include <iostream>

#include "test.h"

class HNetwork_Test: public Test
{
    public:

        void run()
        {
            test_1();
        }

        const char* name()
        {
            return "HNetwork(Reader|Writer)";
        }

    private:

        class TestReader : public HReader<int>
        {
            public:
                virtual int Read(int* dest)
                {
                    static int data[] {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
                    memcpy(dest, data, 14);
                    return 14;
                }

                virtual int Blocksize()
                {
                    return 14;
                }
        };

        void test_1()
        {
            bool terminated = false;


            TestReader rdr;
            HNetworkServer<int> HNetworkServer(1234, &rdr, &terminated);
            ASSERT_IS_EQUAL(1,1);
        }
} hnetwork_test;
