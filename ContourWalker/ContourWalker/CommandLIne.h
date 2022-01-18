#pragma once

#include <string>

class CommandLine
{
	__int64 m_contour;
	bool m_run_tests;
	std::string m_exe;

public:
	
	inline CommandLine(int argc, char* argv[])
	{
		if (argc > 0)
		{
			m_exe = argv[0];
		}

	}
};

