#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>

class CommandLine
{
	__int64 m_contour{ 1 };
	bool m_run_tests{false};
	std::string m_exe;

	enum class Mode
	{
		waiting_for_cmd,
		waiting_for_contour_value,
	};

	inline static std::string ToText(Mode m)
	{
		static std::map<Mode, std::string> names =
		{
			{Mode::waiting_for_cmd, "waiting_for_cmd"},
			{Mode::waiting_for_contour_value, "waiting_for_contour_value"},
		};

		auto it = names.find(m);

		return (it == names.end()) ? "Unknown" : it->second;
	}

public:
	
	CommandLine(int argc, char* argv[])
	{
		if (argc > 0)
		{
			m_exe = argv[0];
		}

		Mode m = Mode::waiting_for_cmd;

		for (int i = 1 ; i < argc ; ++i)
		{
			std::string arg = argv[i];

			switch (m)
			{
			case Mode::waiting_for_cmd:
				if (arg.size() < 2 || (arg[i] != '-' && arg[i] != '/'))
				{
					std::stringstream sstrm;
					sstrm << "Invalid argument: " << arg << std::endl;
					throw std::exception (sstrm.str().c_str());
				}
				switch (arg[1])
				{
				case 't':
					m_run_tests = true;
					break;

				case 'c':
					m = Mode::waiting_for_contour_value;
					break;
				}
				break;

			case Mode::waiting_for_contour_value:
				m_contour = atol(argv[i]);
				if (m_contour <= 1)
				{
					std::stringstream sstrm;
					sstrm << "Invalid contour value: " << arg << std::endl;
					throw std::exception(sstrm.str().c_str());
				}
				break;
			}
		}

		if (m != Mode::waiting_for_cmd)
		{
			std::stringstream sstrm;
			sstrm << "Missing parameter: mode = " << ToText(m) << std::endl;
			throw std::exception(sstrm.str().c_str());
		}
	}

	inline bool RunTests() const { return m_run_tests; }
	inline __int64 Contour() const { return m_contour; }

	inline static void ShowOptions()
	{
		std::cout << "Command line options:" << std::endl;
		std::cout << "  -t: Run tests" << std::endl;
		std::cout << "  -c: <number> Set contour (must be 1 or more)" << std::endl;
	}
};

