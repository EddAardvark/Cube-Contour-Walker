#pragma once

#include <vector>

#include "..\Common\VLInt.h"

class PolynomialIterator
{
	std::vector<VLInt> m_deltas;
	int m_depth;

public:
	inline PolynomialIterator(const std::vector<VLInt>& deltas)
		: m_deltas(deltas)
		, m_depth((int)deltas.size())
	{

	}
	inline PolynomialIterator(const PolynomialIterator & other)
		: m_deltas(other.m_deltas)
		, m_depth(other.m_depth)
	{

	}

	inline const VLInt& Value() const { return m_deltas[0]; }

	inline void Forward()
	{
		for (int i = m_depth - 2; i >= 0; --i)
		{
			m_deltas[i] += m_deltas[i + 1];
		}
	}

	inline void Back()
	{
		for (int i = 0; i < m_depth - 1; ++i)
		{
			m_deltas[i] -= m_deltas[i + 1];
		}
	}
};

