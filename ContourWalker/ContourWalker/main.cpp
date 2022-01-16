// ContourWalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "VLInt.h"
#include "BigCube.h"

void RunTests()
{
    std::cout << "Contour Walker Application!" << std::endl;

    try
    {
        VLInt::Test();
        BigCube::Test();
    }
    catch (std::exception & ex)
    {
        std::cout << "Test failed: " << ex.what() << std::endl;
        exit(-1);
    }
}


int main()
{
    RunTests();
}
