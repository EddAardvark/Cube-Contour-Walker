#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>

#include "../Common/CommandLine.h"


class FilterCommandLine : public CommandLine
{

public:

	FilterCommandLine(int argc, char* argv[])
		: CommandLine(argc, argv)
	{
	}

	const std::string& FileName() const
	{
		return GetString ("f", "");
	}

	const std::string& Exclude() const
	{
		return GetString ("x", "");
	}


};