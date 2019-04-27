#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>


#include "test.h"

std::vector<Test*> tests;

int main(int argc, char **argv)
{
    std::cout << "Running unittests: using Hardt " + getversion() << std::endl; ;
    HInit("hardt_test", false);

    try
    {
        if( Test::execute() )
        {
            std::cout << "One or more tests failed!!" << std::endl;
            return 1;
        }
    }
    catch( std::exception* e )
    {
        std::cout << "Caught exception: " << e->what() << std::endl;
        return 2;
    }
    catch( ... )
    {
        std::cout << "Caught unexpected exception of unknown type!" << std::endl;
        return 3;
    }
}