#include "hardtapi.h"
#include <vector>

class Test;
extern std::vector<Test*> tests;

extern bool run_integration_tests;

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
            int partialFailed = 0;
            for( std::vector<Test*>::iterator it = tests.begin(); it < tests.end(); it++ )
            {
                std::cout << "====================================================================" << std::endl;
                try
                {
                    std::cout << "Test: " << (*it)->name() << std::endl;
                    failed = 0;
                    (*it)->run();
                    partialFailed += failed;
                    std::cout << (failed == 0 ? "OK" : "FAILED") << std::endl;
                }
                catch(std::exception* e)
                {
                    std::cout << "EXCEPTION: " << e->what() << std::endl;
                }
            }
            std::cout << "====================================================================" << std::endl;
            return partialFailed;
        }

    public:

        template <typename T>
        void assertIsEqual(std::string file, int line, T a, T b)
        {
            if( a != b )
            {
                std::cout << "- assertIsEqual(" << (T) a << ", " << (T) b << "): is not equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
            }
        }

        template <typename T>
        void assertIsLessThanOrEqual(std::string file, int line, T a, T b)
        {
            if( a > b )
            {
                std::cout << "- assertIsLessThanOrEqual(" << a << ", " << b << "): is not less than or equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
            }
        }

        template <typename T>
        void assertIsLessThan(std::string file, int line, T a, T b)
        {
            if( a >= b )
            {
                std::cout << "- assertIsLessThan(" << a << ", " << b << "): is not less than!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
            }
        }

        template <typename T>
        void assertIsGreaterThanOrEqual(std::string file, int line, T a, T b)
        {
            if( a < b )
            {
                std::cout << "- assertIsGreaterThanOrEqual(" << a << ", " << b << "): is not greater than or equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
            }
        }

        template <typename T>
        void assertIsGreaterThan(std::string file, int line, T a, T b)
        {
            if( a <= b )
            {
                std::cout << "- assertIsGreaterThan(" << a << ", " << b << "): is not greater than!" << std::endl;
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

#define UNITTEST(a) std::cout << "* Running unittest: " << #a << std::endl; a();
#define INTEGRATIONTEST(a) if( run_integration_tests ) { std::cout << "* Running integrationtest: " << #a << std::endl; a(); } else { std::cout << "- Skipping integrationtest: " << #a << std::endl; }

#define ASSERT_IS_EQUAL(a, b) assertIsEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_LESS_THAN_OR_EQUAL(a, b) assertIsLessThanOrEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_LESS_THAN(a, b) assertIsLessThan(__FILE__,__LINE__, a, b)
#define ASSERT_IS_GREATER_THAN_OR_EQUAL(a, b) assertIsGreaterThanOrEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_GREATER_THAN(a, b) assertIsGreaterThan(__FILE__,__LINE__, a, b)
#define ASSERT_IS_TRUE(a) assertIsTrue(__FILE__,__LINE__, (bool) a)
#define ASSERT_FAIL(reason) assertFail(__FILE__,__LINE__, reason)
#define ASSERT_IGNORE(a) { assertIgnore(__FILE__, __LINE__, a); return; }

#define INFO(a) std::cout << "  " << a << std::endl;
#define WARNING(a) std::cout << "  WARNING: " << a << "!!" << std::endl;