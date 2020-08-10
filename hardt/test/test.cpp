#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>


#include "test.h"

std::vector<Test*> tests;
int Test::failed;
char* Test::sourceDir;

bool run_unit_tests = false;
bool run_integration_tests = false;

int main(int argc, char **argv)
{
    if( argc < 2 )
    {
        std::cout << "Usage: hard_test source-dir-of-hardt_test-sources -it" << argv[1] << std::endl;
        return 1;
    }

    // Should we run unit or integration tests ?
    for( int i = 1; i < argc; i++ )
    {
        if( strcmp(argv[i], "-ut") == 0 )
        {
            run_unit_tests = true;
        }
        if( strcmp(argv[i], "-it") == 0 )
        {
            run_integration_tests = true;
        }
    }

    std::cout << "Running unittests using Hardt " + getversion() << ", sources is at '" << argv[1] << "'" << std::endl;
    Test::sourceDir = argv[1];
    HInit("hardt_test", false);

    try
    {
        if( Test::execute() )
        {
            std::cout << std::endl;
            std::cout << "******************************" << std::endl;
            std::cout << "!! ONE OR MORE TESTS FAILED !!" << std::endl;
            std::cout << "******************************" << std::endl;
            std::cout << std::endl;
            return 2;
        }
    }
    catch( std::exception* e )
    {
        std::cout << "Caught exception: " << e->what() << std::endl;
        return 3;
    }
    catch( ... )
    {
        std::cout << "Caught unexpected exception of unknown type!" << std::endl;
        return 4;
    }
}

HCommand Test::TestNopCommand {HCOMMAND_CLASS::NONE, HCOMMAND_OPCODE::NOP, 0};
