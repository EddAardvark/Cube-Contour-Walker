#pragma once

#include "VLInt.h"

class FourPointCubic
{
	VLInt a, b, c, d;

public:

	FourPointCubic(const VLInt& n1, const VLInt& n2, const VLInt& n3, const VLInt& n4)
	{
		static VLInt six(6);
		static VLInt two(2);

		auto n21 = n2 - n1;
		auto n32 = n3 - n2;
		auto n43 = n4 - n3;

		auto n31 = n32 - n21;
		auto n42 = n43 - n32;

		auto n41 = n42 - n31;

		// n41 = 6a
		// n31 = 6a + 2b
		// n21 = a = b = c
		// n1 = d

		a = n41 / six;
		b = (n31 - n41) / two;
		c = n21 - (a + b);
		d = n1;
	}
	//--------------------------------------------------------------------------------------------
	inline bool IsNatural()const
	{
		static VLInt one (1);

		return a == one;
	}
	//--------------------------------------------------------------------------------------------
	inline VLInt Value(const VLInt & x) const
	{
		return (((((a * x) + b) * x) + c) * x) + d;
	}
	//--------------------------------------------------------------------------------------------
	inline VLInt Value(__int64 x) const
	{
		return (((((a * x) + b) * x) + c) * x) + d;
	}
	//--------------------------------------------------------------------------------------------
	inline bool operator == (const FourPointCubic& other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d;
	}
	//--------------------------------------------------------------------------------------------
	inline bool operator != (const FourPointCubic & other) const
	{
		return a != other.a || b != other.b || c != other.c || d != other.d;
	}
	//=========================================================================================================
	// Monitoring and Testing
	//=========================================================================================================

	static void Test()
	{
		VLInt n1 (1);	// (2x+1) cubed
		VLInt n2 (27);
		VLInt n3 (125);
		VLInt n4 (343);

		FourPointCubic fpc(n1, n2, n3, n4);

		if (fpc.a.ToInt () != 8)
		{
			std::ostringstream sstrm;

			sstrm << "Expected a = 8, got " << fpc;

			throw std::exception(sstrm.str().c_str());
		}

		if (fpc.b.ToInt() != 12)
		{
			std::ostringstream sstrm;

			sstrm << "Expected b = 12, got " << fpc;

			throw std::exception(sstrm.str().c_str());
		}

		if (fpc.c.ToInt() != 6)
		{
			std::ostringstream sstrm;

			sstrm << "Expected c = 6, got " << fpc;

			throw std::exception(sstrm.str().c_str());
		}

		if (fpc.d.ToInt() != 1)
		{
			std::ostringstream sstrm;

			sstrm << "Expected d = 1, got " << fpc;

			throw std::exception(sstrm.str().c_str());
		}

		auto f5 = fpc.Value (VLInt(5)).ToInt();

		if (f5 != 1331)
		{
			std::ostringstream sstrm;

			sstrm << "Expected f(5) = 1331, got " << f5;

			throw std::exception(sstrm.str().c_str());
		}

		// Finished

		std::cout << "FourPointCubic: All tests passed." << std::endl;
	}
	//------------------------------------------------------------------------------------------------------
	inline std::string ToString() const
	{
		std::stringstream sstrm;
		sstrm << "f(x) = " << a << "x^3 + " << b << "x^2 + " << c << "x + " << d;

		return sstrm.str();
	}
	//------------------------------------------------------------------------------------------------------
	inline friend std::ostream& operator << (std::ostream& os, const FourPointCubic& cp)
	{
		return os << cp.ToString();
	}

}; // class


