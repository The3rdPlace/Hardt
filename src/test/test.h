#include "hardtapi.h"
#include <vector>

class Test;
extern std::vector<Test*> tests;

class Test
{
    private:

        static int failed;

    public:

        static char* sourceDir;

    public:

        Test() { tests.push_back(this); }

        static int execute()
        {
            for( std::vector<Test*>::iterator it = tests.begin(); it < tests.end(); it++ )
            {
                std::cout << "====================================================================" << std::endl;
                try
                {
                    std::cout << "Test: " << (*it)->name() << std::endl;
                    failed = 0;
                    (*it)->run();
                    std::cout << (failed == 0 ? "OK" : "FAILED") << std::endl;
                }
                catch(std::exception* e)
                {
                    std::cout << "EXCEPTION: " << e->what() << std::endl;
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
                std::cout << "- assertIsEqual(" << a << ", " << b << "): is not equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
            }
        }

        void assertIsTrue(std::string file, int line, bool a)
        {
            if( a != true )
            {
                std::cout << "- assertIsTrue(" << a << "): is not true!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
            }
        }

        void assertFail(std::string file, int line, std::string reason)
        {
            std::cout << "- assertFail: " << reason << std::endl;
            std::cout << "  in " << file << "@" << line << "\n";
            Test::failed++;
        }

        void assertIgnore(std::string file, int line, std::string reason)
        {
            std::cout << "Ignoring testcase: " << reason << std::endl;
            std::cout << "in " << file << "@" << line << "\n";
        }

    public:

        virtual void run() = 0;
        virtual const char* name() = 0;
};

#define UNITTEST(a) std::cout << "* Running test: " << #a << std::endl; a();

#define ASSERT_IS_EQUAL(a, b) assertIsEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_TRUE(a) assertIsTrue(__FILE__,__LINE__, (bool) a)
#define ASSERT_FAIL(reason) assertFail(__FILE__,__LINE__, reason)
#define ASSERT_IGNORE(a) { assertIgnore(__FILE__, __LINE__, a); return; }