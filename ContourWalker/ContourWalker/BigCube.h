#pragma once

#include "VLInt.h"

//-------------------------------------------------------------------------------------------------
// Implements a big cube using big integers
// Implements increment and decrement operators using decrements to avoid big multiplications.
//
// (c) John Whitehouse 2019 - 2022
// www.eddaardvark.co.uk
//-------------------------------------------------------------------------------------------------


class BigCube
{
    static const __int64 dddy = 6;   // 3rd derivative is constant

    VLInt   root;
    VLInt   value;
    VLInt   dy;
    VLInt   ddy;

    inline BigCube() {}

    inline BigCube(const BigCube& other)
    {
        BigCube ret;

        root = other.root;
        dy = other.dy;
        ddy = other.ddy;
        value = other.value;
    }

public:

    inline static BigCube FromInt(__int64 n)
    {
        auto vli = VLInt::FromInt(n);

        return FromVLInt(vli);
    }

    //-------------------------------------------------------------------------------------------------
    inline static BigCube FromVLInt(const VLInt& n)
    {
        // y = n^3;
        // y' = 3xn^2 + 3xn + 1
        // y'' = 6xn + 6
        // y''' = 6;

        BigCube ret;

        ret.root = VLInt::FromVLInt(n);

        auto n2 = n * n;
        auto n3 = n2 * n;

        ret.value = n3;
        ret.dy = (n2 + n) * 3 + 1;
        ret.ddy = (n + 1) * 6;

        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline static BigCube Clone(const BigCube& other)
    {
        BigCube ret(other);

        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline VLInt GetIncrement() const
    {
        return dy;
    }
    //-------------------------------------------------------------------------------------------------
    inline VLInt GetDecrement() const
    {
        auto temp_ddy = ddy - dddy;
        return dy - temp_ddy;
    }
    //-------------------------------------------------------------------------------------------------
    // Pre increment
    //-------------------------------------------------------------------------------------------------
    inline BigCube& operator ++ ()
    {
        ++root;
        value = value + dy;
        dy = dy + ddy;
        ddy = ddy + dddy;

        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post increment
    //--------------------------------------------------------------------------------------------
    inline BigCube operator ++ (int)
    {
        BigCube temp = *this;

        ++(*this);
        return temp;
    }
    //-------------------------------------------------------------------------------------------------
    // Pre decrement
    //-------------------------------------------------------------------------------------------------
    inline BigCube& operator -- ()
    {
        ddy = ddy - dddy;
        dy = dy - ddy;
        value = value - dy;
        --root;
    }
    //--------------------------------------------------------------------------------------------
    // Post decrement
    //--------------------------------------------------------------------------------------------
    inline BigCube operator -- (int)
    {
        BigCube temp = *this;

        --(*this);
        return temp;
    }
    //-------------------------------------------------------------------------------------------------
    inline BigCube GetNext() const
    {
        BigCube ret = Clone(*this);

        return ++ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline BigCube GetPrevious()
    {
        BigCube ret;

        return --ret;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator > (const BigCube& other)
    {
        return root > other.root;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator >= (const BigCube& other)
    {
        return root >= other.root;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator < (const BigCube& other)
    {
        return root < other.root;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator <= (const BigCube& other)
    {
        return root < other.root;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator == (const BigCube& other)
    {
        return root == other.root;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator != (const BigCube& other)
    {
        return root == other.root;
    }
    //--------------------------------------------------------------------------------------------
    inline bool IsZero() const
    {
        return root.IsZero();
    }
    //=========================================================================================================
    // Monitoring and Testing
    //=========================================================================================================
    std::string toString() const
    {
        std::stringstream sstrm;

        sstrm << "[" << root << "^3 = " << value << "]";
        return sstrm.str();
    }
    //--------------------------------------------------------------------------------------------
    std::string FullText() const
    {
        std::stringstream sstrm;

        sstrm << "BC: n = " << root << ", n^3 = " << value << ", dy = " << dy << ", ddy = " << ddy << ", dddy = 6";
        return sstrm.str();
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const BigCube& bc)
    {
        return os << bc.toString();
    }
    //------------------------------------------------------------------------------------------------------
    inline static void Test()
    {
        BigCube bc = FromInt(1000000000);

        bc.Verify("Test");

        auto vli = VLInt::FromInt(1000000000);
        auto vli3 = vli.Cube();

        if (vli3 != bc.value)
        {
            std::stringstream sstrm;

            sstrm << bc << " != " << vli3;
            throw std::exception(sstrm.str().c_str());
        }

        // Finished

        std::cout << "BigCube: All tests passed." << std::endl;
    }
    //------------------------------------------------------------------------------------------------------
    inline void Verify(const char* where) const
    {
        // y = n^3;
        // y' = 3xn^2 + 3xn + 1
        // y'' = 6xn + 6
        // y''' = 6;

        auto n = root;
        auto n2 = n * n;
        auto val = n2 * n;
        auto t_dy = (n2 + n) * 3 + 1;
        auto t_ddy = (n + 1) * 6;

        if (val != value)
        {
            std::stringstream sstrm;

            sstrm << where << ": Value " << value << " != " << val << ", Detail = " << FullText();
            throw std::exception(sstrm.str().c_str());
        }

        if (t_dy != dy)
        {
            std::stringstream sstrm;

            sstrm << where << ": Dy " << dy << " != " << t_dy << ", Detail = " << FullText();
            throw std::exception(sstrm.str().c_str());
        }

        if (t_ddy != ddy)
        {
            std::stringstream sstrm;

            sstrm << where << ": Ddy " << ddy << " != " << t_ddy << ", Detail = " << FullText();
            throw std::exception(sstrm.str().c_str());
        }
    }

}; // class


