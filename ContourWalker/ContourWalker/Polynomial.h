#pragma once

#include <vector>

#include "..\Common\VLInt.h"

class Polynomial
{
	std::vector<VLInt> m_terms;  // t[0] x^n + t[1] x^(n-1) + ...

public:

	Polynomial(const std::vector<VLInt>& terms)
		: m_terms(terms)
	{

	}
};

