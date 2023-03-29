#pragma once

#include <vector>

#include "..\Common\VLInt.h"

class PolynomialRow
{
	std::vector <VLInt> m_entries;
	int m_max_size;

public:

	PolynomialRow(int n)
		: m_max_size (n)
	{
	}

	inline size_t Size() const { return m_entries.size(); }

	inline const VLInt& operator [] (size_t idx) const
	{
		if (idx >= m_entries.size()) throw std::exception("Index out of range");

		return m_entries[idx];
	}

	inline const VLInt& Head () const
	{
		if (m_entries.empty()) throw std::exception("Index out of range");

		return m_entries[0];
	}

	inline const VLInt& Tail() const
	{
		if (m_entries.empty()) throw std::exception("Index out of range");

		return m_entries[m_entries.size()-1];
	}

	/**
	* When two values are the same we have a potential polynomial
	*/
	inline bool IsConstant () const
	{ 
		return m_entries.size() > 1 && m_entries[0] == m_entries[1];
	}
	/**
	* Remove the first entry
	*/
	inline bool Trim()
	{
		if (m_entries.empty())
		{
			return false;
		}
		m_entries.erase(m_entries.begin());
		return true;
	}
	/**
	* Adds the next element to the row and returns the difference.
	* Returns true if the differentce is set
	*/
	std::pair<bool, VLInt> Add(const VLInt & next)
	{
		m_entries.emplace_back(next);
		auto n = m_entries.size();

		if (n > m_max_size)
		{
			m_entries.erase(m_entries.begin());
			--n;
		}

		if (n > 1)
		{
			return std::make_pair (true, m_entries[n - 1] - m_entries[n - 2]);
		}

		return std::pair<bool, VLInt>();
	}


	//=========================================================================================================
	// Monitoring and Testing
	//=========================================================================================================
	inline std::string ToString() const
	{
		if (m_entries.empty()) return "Empty";

		std::stringstream ret;

		for (const auto& x : m_entries)
		{
			ret << x << " ";
		}
		return ret.str();
	}
	//------------------------------------------------------------------------------------------------------
	inline friend std::ostream& operator << (std::ostream& os, const PolynomialRow& vli)
	{
		return os << vli.ToString();
	}

};

