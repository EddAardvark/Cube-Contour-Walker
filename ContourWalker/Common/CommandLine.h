#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>


class CommandLine
{
	std::map<std::string, std::string> m_settings;

	std::string m_current_key;

	inline static std::string NumericKey(int idx)
	{
		std::stringstream sstrm;

		sstrm << idx;
		return sstrm.str();
	}

public:

	CommandLine(int argc, char* argv[])
	{
		int idx = 0;

		for (int i = 0; i < argc; ++i)
		{
			std::string arg(argv[i]);

			bool is_new_key = !arg.empty() && arg[0] == '-';

			if (is_new_key)
			{
				if (!m_current_key.empty())
				{
					m_settings.emplace(m_current_key, std::string());
				}
				m_current_key = arg.substr(1);

				if (m_current_key.empty())
					throw std::exception("Zero length option");
			}
			else
			{
				if (m_current_key.empty())
				{
					m_settings.emplace(NumericKey(idx), arg);
					++idx;
				}
				else
				{
					m_settings.emplace(m_current_key, arg);
					m_current_key.clear();
				}
			}
		}

		if (!m_current_key.empty())
		{
			m_settings.emplace(m_current_key, std::string());
		}
	}

	void Print() const
	{
		for (const auto& x : m_settings)
		{
			std::cout << x.first << ": " << x.second << std::endl;
		}
	}

	const std::string& GetString(const std::string& key, const std::string& def) const
	{
		auto it = m_settings.find(key);

		return (it != m_settings.end()) ? it->second : def;
	}

	const __int64 GetInteger(const std::string& key, __int64 def) const
	{
		auto it = m_settings.find(key);

		if (it == m_settings.end()) return def;
		
		std::stringstream sstrm(it->second);
		__int64 ret = def;

		sstrm >> ret;

		return ret;
	}

	const bool HasSetting (const std::string& key) const
	{
		auto it = m_settings.find(key);

		return it != m_settings.end();
	}
};