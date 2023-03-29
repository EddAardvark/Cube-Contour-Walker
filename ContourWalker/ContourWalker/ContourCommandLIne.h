#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>

#include "../Common/CommandLine.h"

class ContourCommandLine : public CommandLine
{
	std::string m_exe;

public:
	
	ContourCommandLine(int argc, char* argv[])
		: CommandLine (argc, argv)
	{
	}

	inline bool RunTests() const { return HasSetting ("t"); }
	inline bool ShowHelp() const { return HasSetting("h"); }
	inline bool CheckPolynomials() const { return HasSetting("p"); }

	inline __int64 Contour() const { return GetInteger("c", 2); }
	inline __int64 Iterations() const { return GetInteger("n", 0); }
	inline __int64 ChunkSize() const { return GetInteger("s", 1000); }

	inline std::string FileName() const { return GetString ("f", "?"); }

	inline static void ShowOptions()
	{
		std::cout << "Command line options:" << std::endl;
		std::cout << "  -c: <number> Set contour (must be 1 or more)" << std::endl;
		std::cout << "  -f: <file_name> Set the output file" << std::endl;
		std::cout << "  -h: Show this help" << std::endl;
		std::cout << "  -n: <number> The number of chunks to calculate (0 for run continuously)" << std::endl;
		std::cout << "  -p: Check the values for polynomial trends (slow)" << std::endl;
		std::cout << "  -s: <number> The number of steps in a chunk (must be 1 or more)" << std::endl;
		std::cout << "  -t: Run tests" << std::endl;
	}
};

