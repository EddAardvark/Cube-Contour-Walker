#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "VLInt.h"

//-------------------------------------------------------------------------------------------------
// Implements a number of the form 3nx^2 + 3n^2x + n^3, equivalent to (x+n)^3 - x^3
//
// Uses VLIntegers to avoid being confined to small numbers
//
// (c) John Whitehouse 2021-2022
// www.eddaardvark.co.uk
//-------------------------------------------------------------------------------------------------

class SubCube
{
    VLInt a;        // 3n
    VLInt b;        // 3n^2
    VLInt ax2;      // 2a
    VLInt a_plus_b; // a + b
    VLInt c;        // n^3
    VLInt x;        // Current x
    VLInt n;        // Contour
    VLInt value;    // The value (ax^2 + bx + c)
    VLInt dv;       // 2ax + (a+b+c)
    VLInt ddv;      // 2a


    //-------------------------------------------------------------------------------------------------
    inline SubCube()
    {
    }

    //-------------------------------------------------------------------------------------------------
    inline SubCube(const SubCube& other)
    {
        a = other.a;
        b = other.b;
        ax2 = other.ax2;
        a_plus_b = other.a_plus_b;
        c = other.c;
        x = other.x;
        n = other.n;
        value = other.value;
        dv = other.dv;
        ddv = other.ddv;
    }

public:


    //-------------------------------------------------------------------------------------------------
    inline static SubCube FromInts(__int64 x, __int64 n)
    {
        return FromVLInts(VLInt::FromInt(x), VLInt::FromInt(n));
    }
    //-------------------------------------------------------------------------------------------------
    inline static SubCube FromVLInts(const VLInt& x, const VLInt& contour)
    {
        // n = contour
        // v = 3nx^2 + 3n^2x + n^3, equivalent to ax^2 + bx + c, where
        // a = 3n, b = 3n^2, c = n^3
        // v' = +3xn^2 + 3xn + 1
        // v'' = 6xn + 6

        SubCube ret;

        ret.x = x;
        ret.n = contour;

        auto n2 = ret.n * ret.n;

        ret.a = ret.n * 3;
        ret.b = n2 * 3;
        ret.c = n2 * ret.n;
        ret.ax2 = ret.a * 2;
        ret.a_plus_b = ret.a + ret.b;
        ret.dv = ret.ax2 * x + ret.a_plus_b;
        ret.ddv = ret.ax2;

        ret.value = ret.CalculateValue();

        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline VLInt CalculateValue () const
    {
        return ((a * x) + b) * x + c;
    }
    //--------------------------------------------------------------------------------------------
    inline SubCube& operator = (const SubCube& other)
    {
        if (this != &other)
        {
            a = other.a;
            b = other.b;
            c = other.c;
            x = other.x;
            n = other.n;
            value = other.value;
            dv = other.dv;
            ddv = other.ddv;
        }

        return (*this);
    }
    //-------------------------------------------------------------------------------------------------
    inline void Hop (__int64 hop)
    {
        x = x + hop;

        value = CalculateValue();
        dv = a * x * 2 + (a + b);
        ddv = a * 2;
    }
    //--------------------------------------------------------------------------------------------
    // Pre increment
    //--------------------------------------------------------------------------------------------
    inline SubCube& operator ++ ()
    {
        ++ x;
        value = value + dv;
        dv = dv + ddv;
        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post increment
    //--------------------------------------------------------------------------------------------
    inline SubCube operator ++ (int)
    {
        SubCube temp = *this;

        ++(*this);
        return temp;
    }
    //--------------------------------------------------------------------------------------------
    // Pre decrement
    //--------------------------------------------------------------------------------------------
    inline SubCube& operator -- ()
    {
        -- x;
        dv = dv - ddv;
        value = value - dv;

        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post decrement
    //--------------------------------------------------------------------------------------------
    inline SubCube operator -- (int)
    {
        SubCube temp = *this;

        --(*this);
        return temp;
    }
    //=========================================================================================================
    // Monitoring and Testing
    //=========================================================================================================
    inline std::string toString () const
    {
        std::stringstream ret;

        ret << "[SC(" << x << "," << n << ") = " << value << "]";

        return ret.str();
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const SubCube& vli)
    {
        return os << vli.toString();
    }
    //--------------------------------------------------------------------------------------------
    inline std::string FullText (const char * where) const
    {
        std::stringstream ret;

        ret << "SC(" << x << ", " << n << ") = " << value << ", DV = " << dv << ", DDV = " << ddv;

        return ret.str();
    }
    //--------------------------------------------------------------------------------------------
    inline void Verify(const char* where) const
    {
        auto good = SubCube::FromVLInts(x, n);

        if (good.value != value)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: value " << value << " != " << good.value;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.dv != dv)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: dv " << dv << " != " << good.dv;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.ddv != ddv)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: ddv " << ddv << " != " << good.ddv;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.a != a)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: a " << a << " != " << good.a;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.b != b)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: b " << b << " != " << good.b;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.c != c)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: c " << c << " != " << good.c;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.ax2 != ax2)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: ax2 " << ax2 << " != " << good.ax2;
            throw std::exception(sstrm.str().c_str());
        }

        if (good.a_plus_b != a_plus_b)
        {
            std::stringstream sstrm;
            sstrm << where << ": SubCube: a_plus_b " << a_plus_b << " != " << good.a_plus_b;
            throw std::exception(sstrm.str().c_str());
        }
    }
    //--------------------------------------------------------------------------------------------
    inline static void Test ()
    {
        auto sc = FromInts(1, 1);

        if (sc.value.ToInt () != 7) throw std::exception("SC(1,1) value");
        if (sc.dv.ToInt() != 12) throw std::exception ("SC(1,1) dv");
        if (sc.ddv.ToInt() != 6) throw std::exception ("SC(1,1) ddv");


        ++ sc;
        if (sc.value.ToInt() != 19) throw std::exception("SC(1,1) inc1");

        ++ sc;
        if (sc.value.ToInt() != 37) throw std::exception("SC(1,1) inc2");

        ++ sc;
        if (sc.value.ToInt() != 61) throw std::exception("SC(1,1) inc3");

        -- sc;
        if (sc.value.ToInt() != 37) throw std::exception("SC(1,1) dec1");

        -- sc;
        if (sc.value.ToInt() != 19) throw std::exception("SC(1,1) dec2");

        -- sc;
        if (sc.value.ToInt() != 7) throw std::exception("SC(1,1) dec3");

        -- sc;
        if (sc.value.ToInt() != 1) throw std::exception("SC(1,1) dec4");

        // n = 4

        sc = FromInts(1, 4);

        if (sc.value.ToInt() != 124) throw std::exception("SC(1,4) value");

        ++ sc;
        if (sc.value.ToInt() != 208) throw std::exception("SC(1,4) inc1");

        ++ sc;
        if (sc.value.ToInt() != 316) throw std::exception("SC(1,4) inc2");

        ++ sc;
        if (sc.value.ToInt() != 448) throw std::exception("SC(1,4) inc3");

        -- sc;
        if (sc.value.ToInt() != 316) throw std::exception("SC(1,4) dec1");

        -- sc;
        if (sc.value.ToInt() != 208) throw std::exception("SC(1,4) dec2");

        -- sc;
        if (sc.value.ToInt() != 124) throw std::exception("SC(1,4) dec3");

        -- sc;
        if (sc.value.ToInt() != 64) throw std::exception("SC(1,4) dec4");

        // Hop

        auto sc1 = FromInts(2, 11);
        auto sc2 = FromInts(25, 11);

        sc1.Hop(23);
        sc1.Verify("hop sc1");
        sc2.Verify("hop sc2");

        if (sc1.value != sc2.value)
        {
            std::stringstream sstrm;
            sstrm << "Hop [23]: " << sc1 << " != " << sc2;
            throw std::exception(sstrm.str().c_str());
        }

        // Post inc

        SubCube sc3 = sc2++;

        sc2.Verify("post inc (2)");
        sc3.Verify("post inc (3)");

        if (sc2.x.ToInt () != 26)
        {
            std::stringstream sstrm;
            sstrm << "Post inc (a): " << sc2 << ", x != 26";
            throw std::exception(sstrm.str().c_str());
        }
        if (sc3.x.ToInt() != 25)
        {
            std::stringstream sstrm;
            sstrm << "Post inc (b): " << sc3 << ", x != 25";
            throw std::exception(sstrm.str().c_str());
        }

        // Post dec

        SubCube sc4 = sc3--;

        sc3.Verify("post dec (3)");
        sc4.Verify("post dec (4)");

        if (sc3.x.ToInt() != 24)
        {
            std::stringstream sstrm;
            sstrm << "Post inc (a): " << sc3 << ", x != 24";
            throw std::exception(sstrm.str().c_str());
        }
        if (sc4.x.ToInt() != 25)
        {
            std::stringstream sstrm;
            sstrm << "Post inc (b): " << sc4 << ", x != 25";
            throw std::exception(sstrm.str().c_str());
        }
        // Finished

        std::cout << "SubCube: All tests passed." << std::endl;

    }

};