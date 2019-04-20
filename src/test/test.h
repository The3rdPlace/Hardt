#include "hardtapi.h"
#include <vector>

class Test;
extern std::vector<Test*> tests;

class Test
{
    public:

        Test() { tests.push_back(this); }

        static int execute()
        {
            int failed = 0;
            for( std::vector<Test*>::iterator it = tests.begin(); it < tests.end(); it++ )
            {
                std::cout << "====================================================================" << std::endl;
                try
                {
                    std::cout << "Test: " << (*it)->name() << std::endl;
                    (*it)->run();
                    std::cout << "OK" << std::endl;
                }
                catch(std::exception*)
                {
                    failed++;
                    std::cout << "FAILED" << std::endl;
                }
            }
            std::cout << "====================================================================" << std::endl;
            return failed;
        }

    public:

        template <typename T>
        void assertIsEqual(std::string file, int line, T a, T b)
        {
            if( a != b )
            {
                std::cout << "assertIsEqual(" << a << ", " << b << "): is not equal!" << std::endl;
                std::cout << "in " << file << "@" << line << std::endl;
                throw new std::exception();
            }
        }

        void assertFail(std::string file, int line, std::string reason)
        {
            std::cout << "assertFail: " << reason << std::endl;
            std::cout << "in " << file << "@" << line << "\n";
            throw new std::exception();
        }

    public:

        virtual void run() = 0;
        virtual const char* name() = 0;
};

#define ASSERT_IS_EQUAL(a, b) assertIsEqual(__FILE__,__LINE__, a, b)
#define ASSERT_FAIL(reason) assertFail(__FILE__,__LINE__, reason)
