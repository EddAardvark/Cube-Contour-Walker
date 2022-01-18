// ContourWalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "VLInt.h"
#include "BigCube.h"
#include "SubCube.h"
#include "CommandLine.h"

void RunTests()
{
    std::cout << "Contour Walker Application!" << std::endl;

    try
    {
        VLInt::Test();
        BigCube::Test();
        SubCube::Test();
    }
    catch (std::exception & ex)
    {
        std::cout << "Test failed: " << ex.what() << std::endl;
        exit(-1);
    }
}


void RunCalculation(__int64 c)
{
    std::cout << "Contour = " << c << std::endl;
}


int main(int argc, char* argv[])
{
    try
    {
        CommandLine cmd (argc, argv);

        if (cmd.RunTests())
        {
            RunTests();
        }
        RunCalculation(cmd.Contour());
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        CommandLine::ShowOptions();
        exit(-1);
    }

    exit(0);
    return 0;
}
