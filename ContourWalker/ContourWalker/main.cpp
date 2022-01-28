// ContourWalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "VLUInt.h"
#include "VLInt.h"
#include "BigCube.h"
#include "SubCube.h"
#include "CommandLine.h"
#include "ContourWalker.h"
#include "FourPointCubic.h"


void RunTests()
{
    std::cout << "Contour Walker Application!" << std::endl;

    try
    {
        VLUInt::Test();
        VLInt::Test();
        BigCube::Test();
        SubCube::Test();
        FourPointCubic::Test();
    }
    catch (std::exception & ex)
    {
        std::cout << "Test failed: " << ex.what() << std::endl;
        exit(-1);
    }
}


void RunCalculation(__int64 contour, __int64 steps, __int64 chunk_size)
{
    std::cout << "Contour = " << contour << std::endl;
    if (steps == 0)
        std::cout << "Steps = run forever" << std::endl;
    else
        std::cout << "Steps = " << steps << std::endl;
    std::cout << "Chunk Size = " << chunk_size << std::endl;

    ContourWalker walker(contour, steps, chunk_size);

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
        CommandLine cmd (argc, argv);

        if (cmd.ShowHelp())
        {
            CommandLine::ShowOptions();
            exit(0);
        }

        if (cmd.RunTests())
        {
            RunTests();
        }
        RunCalculation(cmd.Contour(), cmd.Iterations (), cmd.ChunkSize());
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
