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
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const BigCube& bc)
    {
        return os << bc.ToString();
    }
    //------------------------------------------------------------------------------------------------------
    static void Test();
    void Verify(const char* where) const;
    std::string FullText() const;

}; // class


