#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>


#include "test.h"

std::vector<Test*> tests;
int Test::failed;
char* Test::sourceDir;

int main(int argc, char **argv)
{
    if( argc < 2 )
    {
        std::cout << "Usage: hard_test source-dir-of-hardt_test-sources" << argv[1] << std::endl;
        return 1;
    }

    std::cout << "Running unittests using Hardt " + getversion() << ", sources is at '" << argv[1] << "'" << std::endl;
    Test::sourceDir = argv[1];
    HInit("hardt_test", true);

    try
    {
        if( Test::execute() )
        {
            std::cout << "One or more tests failed!!" << std::endl;
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