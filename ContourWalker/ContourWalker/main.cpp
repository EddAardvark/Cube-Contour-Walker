// ContourWalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "..\Common\VLUInt.h"
#include "..\Common\VLInt.h"
#include "..\Common\BigCube.h"
#include "SubCube.h"
#include "ContourCommandLine.h"
#include "ContourWalker.h"
#include "PolynomialFitter.h"


void RunTests()
{
    std::cout << "Contour Walker Application!" << std::endl;

    try
    {
        VLUInt::Test();
        VLInt::Test();
        BigCube::Test();
        SubCube::Test();
        PolynomialFitter::Test();
    }
    catch (std::exception & ex)
    {
        std::cout << "Test failed: " << ex.what() << std::endl;
        exit(-1);
    }
}


void RunCalculation(const ContourCommandLine& cmd)
{
    ContourWalker walker(cmd);

    time_t now;
        
    time(&now);
    walker.Walk();
    time_t now2;
    time(&now2);

    std::cout << "Duration : " << (now2 - now) << std::endl;
}


int main(int argc, char* argv[])
{
    try
    {
        ContourCommandLine cmd (argc, argv);

        if (cmd.ShowHelp())
        {
            ContourCommandLine::ShowOptions();
            exit(0);
        }

        if (cmd.RunTests())
        {
            RunTests();
        }
        RunCalculation(cmd);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        ContourCommandLine::ShowOptions();
        exit(-1);
    }

    exit(0);
    return 0;
}
