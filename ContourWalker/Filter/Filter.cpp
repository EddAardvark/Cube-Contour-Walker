// Filter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

#include "FilterCommandLine.h"

int main(int argc, char* argv[])
{
    FilterCommandLine cmd(argc, argv);

    cmd.Print();

    std::string fname = cmd.FileName();
    std::string exclude = cmd.Exclude();

    int n = 0;

    try
    {
        std::ifstream input;

        input.open(fname, std::ios_base::in);

        if (! input.is_open())
        {
            std::cout << "Can't open " << fname << std::endl;
            exit(0);
        }

        while (!input.eof())
        {
            char buffer[1024];
            
            input.getline(buffer, sizeof(buffer)-1);
            ++n;

            std::string line (buffer);

            auto pos = line.find(exclude);

            if (pos == std::string::npos)
                std::cout << line << std::endl;
        }
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }
    std::cout << n << " lines read" << std::endl;
    exit(0);
}
