#pragma once

#include <vector>

#include "PolynomialRow.h"
#include "PolynomialIterator.h"

class PolynomialFitter
{
	int m_depth;
	std::vector<PolynomialRow> m_rows;


public:

	inline PolynomialFitter(int depth)
		: m_depth(depth)
	{
		for (int i = 0; i < depth - 1; ++i)
		{
			m_rows.emplace_back(PolynomialRow(depth - i));
		}
	}

	inline void Add(const VLInt& x)
	{
		auto next = m_rows[0].Add(x);

		for (auto i = 1 ; i < m_rows.size() - 1 && next.first ; ++i)
		{
			next = m_rows[i].Add(next.second);
		}
	}
	//------------------------------------------------------------------------------------------------------
	inline int Search() const
	{
		for (auto i = 0; i < m_rows.size() - 1; ++i)
		{
			if (m_rows[i].IsConstant())
			{
				return i + 1;
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------------------------------
	inline int Trim()
	{
		for (auto i = 0; i < m_rows.size() - 1; ++i)
		{
			if (!m_rows[i].Trim())
			{
				break;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	inline PolynomialIterator GetIterator(int order) const
	{
		if (order >= m_rows.size()) throw std::exception ("polynomial order too large");

		std::vector <VLInt> terms;

		for (int i = 0; i < order + 1 ; ++i)
		{
			terms.emplace_back(m_rows[i].Tail());
		}
		return PolynomialIterator(terms);
	}
	//------------------------------------------------------------------------------------------------------
	inline std::vector <VLInt> GetLinear() const
	{
		std::vector <VLInt> ret;

		ret.emplace_back(m_rows[1].Head());	// a in ax+b
		ret.emplace_back(m_rows[0].Head()); // b

		return ret;
	}
	//------------------------------------------------------------------------------------------------------
	// a = n20/2, b = n10 - a, c = n00
	inline std::vector <VLInt> GetQuadratic() const
	{
		std::vector <VLInt> ret;

		auto a = m_rows[2].Head().DivideInt (2);
		
		ret.emplace_back(a);					// a in ax^2+bx+c
		ret.emplace_back(m_rows[1].Head()-a);	// a in ax+b
		ret.emplace_back(m_rows[0].Head());		// c

		return ret;
	}
	//------------------------------------------------------------------------------------------------------
	// a = n20/2, b = n10 - a, c = n00
	inline std::vector <VLInt> GetCubic() const
	{
		std::vector <VLInt> ret;

		auto a = m_rows[3].Head().DivideInt (6);
		auto b = (m_rows[2].Head() - m_rows[3].Head()).DivideInt(2);
		auto c = m_rows[1].Head() - (a + b);

		ret.emplace_back(a);					// a in ax^2+bx+c
		ret.emplace_back(b);					// a in ax^2+bx+c
		ret.emplace_back(c);					// a in ax^2+bx+c
		ret.emplace_back(m_rows[0].Head());		// a in ax^2+bx+c

		return ret;
	}
	//=========================================================================================================
	// Monitoring and Testing
	//=========================================================================================================

	void Print() const
	{
		std::cout << "PolynomialFitter: rows = " << m_rows.size() << ", depth = " << m_depth << std::endl;

		for (int i = 0; i < m_rows.size(); ++i)
		{
			std::cout << "    Row[" << i << "] = " << m_rows[i] << std::endl;
		}
	}

	static void Test()
	{
		std::vector<VLInt> poly{ 1,27,125,343 }; // (2x=1)^3

		PolynomialFitter fitter(10);

		for (const auto& x : poly)
		{
			fitter.Add(x);
		}

		PolynomialIterator iterator = fitter.GetIterator(3);	// 3 = cubic

		if (iterator.Value().ToInt() != 343)
		{
			std::ostringstream sstrm;

			sstrm << "Expected iterator value = 343, got " << iterator.Value();

			throw std::exception(sstrm.str().c_str());
		}

		static int expectation[] = { 729, 1331, 2197 };

		for (int i = 0; i < 3; ++i)
		{
			iterator.Forward();

			if (iterator.Value().ToInt() != expectation[i])
			{
				std::ostringstream sstrm;

				sstrm << "Expected iterator value = " << expectation[i] << ", got " << iterator.Value();

				throw std::exception(sstrm.str().c_str());
			}
		}

		static int expect2[] = { 1331, 729, 343, 125, 27, 1, -1, -27, -125 };

		for (int i = 0; i < 9; ++i)
		{
			iterator.Back();

			if (iterator.Value().ToInt() != expect2[i])
			{
				std::ostringstream sstrm;

				sstrm << "Expected iterator value = " << expect2[i] << ", got " << iterator.Value();

				throw std::exception(sstrm.str().c_str());
			}
		}

		// Finished

		std::cout << "PolynomialFitter: All tests passed." << std::endl;
	}
	//------------------------------------------------------------------------------------------------------
	inline std::string ToString() const
	{
		std::stringstream sstrm;
		sstrm << "Filter: D = " << m_depth;

		return sstrm.str();
	}
	//------------------------------------------------------------------------------------------------------
	inline friend std::ostream& operator << (std::ostream& os, const PolynomialFitter& pf)
	{
		return os << pf.ToString();
	}

}; // class

