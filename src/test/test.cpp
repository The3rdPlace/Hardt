#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>

#include "../hardt/include/hardtapi.h"

class Test
{
    private:

        static std::vector<Test*> tests;

    public:

        Test() { tests.push_back(this); }

        static int execute()
        {
            int failed = 0;
            for( std::vector<Test*>::iterator it = tests.begin(); it < tests.end(); it++ )
            {
                try
                {
                    (*it)->run();
                }
                catch(std::exception*)
                {
                    failed++;
                }
            }
            return failed;
        }

    public:

        template <typename T>
        void assertIsEqual(std::string file, int line, T a, T b)
        {
            if( a != b )
            {
                std::cout << "====================================================================" << std::endl;
                std::cout << "assertIsEqual(" << a << ", " << b << "): is not equal!" << std::endl;
                std::cout << "in " << file << "@" << line << std::endl;
                throw new std::exception();
            }
        }

        void assertFail(std::string file, int line, std::string reason)
        {
            std::cout << "====================================================================" << std::endl;
            std::cout << "assertFail: " << reason << std::endl;
            std::cout << "in " << file << "@" << line << "\n";
            throw new std::exception();
        }

    public:

        virtual void run() = 0;
};

std::vector<Test*> Test::tests;

#define ASSERT_IS_EQUAL(a, b) assertIsEqual(__FILE__,__LINE__, a, b)
#define ASSERT_FAIL(reason) assertFail(__FILE__,__LINE__, reason)

#include "test.h"

int main(int argc, char **argv)
{
    std::cout << "Running unittests: using Hardt " + getversion() << "\n" ;

    if( Test::execute() )
    {
        std::cout << "====================================================================\n";
        std::cout << "One or more tests failed!!\n";
        std::cout << "====================================================================\n";
    }
}