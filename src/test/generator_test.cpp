#include <stdio.h>
#include <iostream>

class Generator_Test: public Test
{
    public:

        void run()
        {
            test_1();
        }

    private:

        void test_1()
        {
            ASSERT_IS_EQUAL(1,2);
        }
} test;
