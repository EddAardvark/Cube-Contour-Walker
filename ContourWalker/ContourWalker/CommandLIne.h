#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>

class CommandLine
{
	__int64 m_contour{ 1 };
	__int64 m_iterations{ 0 };
	__int64 m_chunk_size{ 500 };
	bool m_run_tests{false};
	bool m_show_help{false};

	std::string m_exe;

	enum class Mode
	{
		waiting_for_cmd,
		waiting_for_contour_value,
		waiting_for_max_value,
		waiting_for_chunk,
	};

	inline static std::string ToText(Mode m)
	{
		static std::map<Mode, std::string> names =
		{
			{Mode::waiting_for_cmd, "waiting_for_cmd"},
			{Mode::waiting_for_contour_value, "waiting_for_contour_value"},
			{Mode::waiting_for_max_value, "waiting_for_max_value"},
			{Mode::waiting_for_chunk, "waiting_for_chunk"},
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
				if (arg.size() < 2 || (arg[0] != '-' && arg[0] != '/'))
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

				case 'n':
					m = Mode::waiting_for_max_value;
					break;

				case 's':
					m = Mode::waiting_for_chunk;
					break;

				case 'h':
					m_show_help = true;
					return;
				}
				break;

			case Mode::waiting_for_contour_value:
				m_contour = atol(argv[i]);
				m = Mode::waiting_for_cmd;

				if (m_contour <= 1)
				{
					std::stringstream sstrm;
					sstrm << "Invalid contour value: " << arg << std::endl;
					throw std::exception(sstrm.str().c_str());
				}
				break;

			case Mode::waiting_for_max_value:
				m_iterations = atol(argv[i]);
				m = Mode::waiting_for_cmd;

				if (m_iterations < 0)
				{
					std::stringstream sstrm;
					sstrm << "Invalid num steps value: " << arg << std::endl;
					throw std::exception(sstrm.str().c_str());
				}
				break;

			case Mode::waiting_for_chunk:
				m_chunk_size = atol(argv[i]);
				m = Mode::waiting_for_cmd;

				if (m_chunk_size <= 1)
				{
					std::stringstream sstrm;
					sstrm << "Invalid chunk size: " << arg << std::endl;
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
	inline bool ShowHelp() const { return m_show_help; }
	inline __int64 Contour() const { return m_contour; }
	inline __int64 Iterations() const { return m_iterations; }
	inline __int64 ChunkSize() const { return m_chunk_size; }

	inline static void ShowOptions()
	{
		std::cout << "Command line options:" << std::endl;
		std::cout << "  -c: <number> Set contour (must be 1 or more)" << std::endl;
		std::cout << "  -h: Show this help" << std::endl;
		std::cout << "  -n: <number> The number of chunks to calculate (0 for run continuously)" << std::endl;
		std::cout << "  -s: <number> The number of steps in a chunk (must be 1 or more)" << std::endl;
		std::cout << "  -t: Run tests" << std::endl;
	}
};

