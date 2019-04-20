#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>


#include "test.h"

std::vector<Test*> tests;

int main(int argc, char **argv)
{
    std::cout << "Running unittests: using Hardt " + getversion() << std::endl; ;
    HInit("hardt_test");

    if( Test::execute() )
    {
        std::cout << "One or more tests failed!!" << std::endl;
    }
}