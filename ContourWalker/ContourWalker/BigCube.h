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

    VLInt   ddy;

public:

    VLInt   root;
    VLInt   value;
    VLInt   dy;

    inline BigCube() {}

    inline BigCube(const BigCube& other)
    {
        BigCube ret;

        root = other.root;
        dy = other.dy;
        ddy = other.ddy;
        value = other.value;
    }

    inline BigCube (__int64 n)
    {
        root = VLInt (n);

        Inflate();
    }
    //-------------------------------------------------------------------------------------------------
    inline BigCube (const VLInt& n)
    {
        root = n;

        Inflate();
    }
    //-------------------------------------------------------------------------------------------------
    inline void Inflate ()
    {
        // y = n^3;
        // y' = 3xn^2 + 3xn + 1
        // y'' = 6xn + 6
        // y''' = 6;

        auto n2 = root * root;
        auto n3 = n2 * root;

        value = n3;
        dy = (n2 + root) * 3 + 1;
        ddy = (root + 1) * 6;
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
        BigCube ret = *this;

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
    std::string ToString() const
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
        return os << bc.ToString();
    }
    //------------------------------------------------------------------------------------------------------
    inline static void Test()
    {
        BigCube bc (1000000000);

        bc.Verify("Test 1");

        VLInt vli (1000000000);
        auto vli3 = vli.Cube();

        if (vli3 != bc.value)
        {
            std::stringstream sstrm;

            sstrm << bc << " != " << vli3;
            throw std::exception(sstrm.str().c_str());
        }

        VLInt eleven(11);
        BigCube bc2(eleven);

        if (bc2.value != 1331)
        {
            std::stringstream sstrm;

            sstrm << "11^3 != 1331 , got " << bc2;
            throw std::exception(sstrm.str().c_str());
        }

        VLInt m20(-20);
        BigCube bc3(m20);

        if (bc3.value != -8000)
        {
            std::stringstream sstrm;

            sstrm << "(-20)^3 != -8000 , got " << bc3;
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


