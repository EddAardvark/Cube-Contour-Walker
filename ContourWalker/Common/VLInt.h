#pragma once

//-------------------------------------------------------------------------------------------------
// Stores very large integers as a set of smaller ones (base 10^N)
// 
// (c) John Whitehouse 2019-2022
// www.eddaardvark.co.uk
//-------------------------------------------------------------------------------------------------

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <iostream>

#include "VLUInt.h"

class VLInt
{
public:

    bool positive{ true };      // Sign
    VLUInt value;               // Unsigned value

    int length{ 0 };
    

    inline VLInt() : value(0), positive(true) {}

    //----------------------------------------------------------------------------------------------------------------
    inline VLInt (__int64 n)
        : positive (n >= 0)
        , value (abs(n))
    {
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt (const VLInt & other)
        : positive (other.positive)
        , value (other.value)
    {
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt(const VLUInt& val, bool pstve)
        : positive(pstve)
        , value (val)
    {
    }
    //--------------------------------------------------------------------------------------------
    inline bool TestSmall(int target) const
    {
        return value.TestSmall(target);
    }
    //--------------------------------------------------------------------------------------------
    inline const VLInt & operator = (const VLInt& other)
    {
        if (this != &other)
        {
            positive = other.positive;
            value = other.value;
        }

        return (* this);
    }
    //--------------------------------------------------------------------------------------------
    inline const VLInt& operator = (__int64 other)
    {
        positive = other >= 0;
        value = abs(other);

        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Multiply by a simple integer
    //--------------------------------------------------------------------------------------------
    inline VLInt operator * (__int64 num) const
    {
        VLUInt val = value * num;
        bool pstve = positive == (num >= 0);

        return VLInt (val, pstve);
    }
    //--------------------------------------------------------------------------------------------
    // Multiply by a another big integer
    //--------------------------------------------------------------------------------------------
    inline VLInt operator * (const VLInt & other) const
    {
        VLUInt val = value * other.value;
        bool pstve = positive == other.positive;

        return VLInt(val, pstve);
    }
    //--------------------------------------------------------------------------------------------
    // Add a simple number (this number is truncated to an integer)
    //--------------------------------------------------------------------------------------------
    inline VLInt operator + (__int64 num) const
    {
        auto n = VLInt(num);

        return (*this) + n;
    }
    //--------------------------------------------------------------------------------------------
    // Subtract a simple number
    //--------------------------------------------------------------------------------------------
    inline VLInt operator - (__int64 num) const
    {
        auto n = VLInt(num);

        return (*this) - n;
    }
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    inline const VLInt& operator += (const VLInt& other)
    {
        return (*this) = (*this) + other;
    }
    //--------------------------------------------------------------------------------------------
    // Add a simple number (this number is truncated to an integer)
    // TODO: Optimise using values directly
    //--------------------------------------------------------------------------------------------
    inline const VLInt& operator -= (const VLInt& other)
    {
        return (*this) = (*this) - other;
    }
    //--------------------------------------------------------------------------------------------
    // Pre increment
    //--------------------------------------------------------------------------------------------
    inline VLInt & operator ++ ()
    {
        if (IsZero())
        {
            positive = true;
            value = 1;
            return (*this);
        }

        if (positive)
        {
            ++ value;
            return (*this);
        }

        --value;
        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post increment
    //--------------------------------------------------------------------------------------------
    inline VLInt operator ++ (int)
    {
        VLInt temp = *this;
        
        ++ (*this);
        return temp;
    }
    //--------------------------------------------------------------------------------------------
    // Pre decrement
    //--------------------------------------------------------------------------------------------
    inline VLInt& operator -- ()
    {
        if (IsZero())
        {
            positive = false;
            value = 1;
            return (*this);
        }

        if (positive)
        {
            --value;
            return (*this);
        }

        ++value;
        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post decrement
    //--------------------------------------------------------------------------------------------
    inline VLInt operator -- (int)
    {
        VLInt temp = *this;

        --(*this);
        return temp;
    }
    //--------------------------------------------------------------------------------------------
    // Add
    //--------------------------------------------------------------------------------------------
    inline VLInt operator + (const VLInt & other) const
    {
        if (other.positive == positive)
        {
            VLUInt val = value + other.value;
            return VLInt (val, positive);
        }

        auto comp = VLUInt::Compare (value, other.value);

        if (comp == 0) // A + (-A) = 0
        {
            return VLInt(0);
        }

        if (comp > 0)
        {
            VLUInt val = value - other.value;
            return VLInt(val, positive);
        }

        VLUInt val = other.value - value;
        return VLInt(val, other.positive);
    }
    //--------------------------------------------------------------------------------------------
    // Subtract
    //--------------------------------------------------------------------------------------------
    inline VLInt operator - (const VLInt& other) const
    {
        if (other.positive != positive)
        {
            VLUInt val = value + other.value;
            return VLInt(val, positive);
        }

        auto comp = VLUInt::Compare(value, other.value);

        if (comp == 0) // A - A = 0
        {
            return VLInt(0);
        }

        if (comp > 0)
        {
            VLUInt val = value - other.value;
            return VLInt(val, positive);
        }


        VLUInt val = other.value - value;
        return VLInt(val, ! other.positive);
    }
    //--------------------------------------------------------------------------------------------
    // Compare, returns -1, 0 or 1 for (first < second), (first == second) and (first > second)
    //--------------------------------------------------------------------------------------------
    inline static int Compare (const VLInt & first, const VLInt& second)
    {
        if (first.positive != second.positive)
        {
            return first.positive ? 1 : -1;
        }

        auto c = VLUInt::Compare(first.value, second.value);

        return first.positive ? c : -c;
    }
    //--------------------------------------------------------------------------------------------
    // returns the minimum (by reference)
    //--------------------------------------------------------------------------------------------
    inline static const VLInt & Min (const VLInt& first, const VLInt& second)
    {
        return (Compare(first, second) > 0) ? second : first;
    }
    //--------------------------------------------------------------------------------------------
    // returns the maximum (by reference)
    //--------------------------------------------------------------------------------------------
    inline static const VLInt & Max(const VLInt& first, const VLInt& second)
    {
        return (Compare(first, second) > 0) ? first : second;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator > (const VLInt& other) const
    {
        return Compare((*this), other) > 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator >= (const VLInt& other) const
    {
        return Compare((*this), other) >= 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator < (const VLInt& other) const
    {
        return Compare((*this), other) < 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator <= (const VLInt& other) const
    {
        return Compare((*this), other) <= 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator == (const VLInt& other) const
    {
        return Compare((*this), other) == 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator != (const VLInt& other) const
    {
        return Compare((*this), other) != 0;
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt Abs () const
    {
        return VLInt(value, true);
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt operator - () const
    {
        return VLInt(value, !positive);
    }
    //--------------------------------------------------------------------------------------------
    inline bool IsZero () const
    {
        return value.IsZero();
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod2 () const { return value.Mod2(); }
    inline int Mod3() const { return value.Mod3(); }
    inline int Mod4() const { return value.Mod4(); }
    inline int Mod5() const { return value.Mod5(); }
    inline int Mod8() const { return value.Mod8(); }
    inline int Mod9() const { return value.Mod9(); }
    inline int Mod10() const { return value.Mod10(); }
    inline int Mod16() const { return value.Mod16(); }
    
    //--------------------------------------------------------------------------------------------
    // Divide by an integer (< BASE)
    inline VLInt DivideInt (__int64 number) const
    {
        VLUInt val = value / abs(number);
        bool pstve = positive == (number >= 0);

        return VLInt(val, pstve);
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt operator % (__int64 number) const
    {
        VLUInt val = value.DivMod(number).second;

        return VLInt(val, true);
    }

    // Divide, this/other, returns a VLInt

    inline VLInt operator / (const VLInt& other)
    {
        if (other.IsZero())
        {
            throw std::invalid_argument("Divide by zero");
        }

        VLUInt val = value / other.value;
        bool pstve = positive == other.positive;
        return VLInt(val, pstve);
    }

    //------------------------------------------------------------------------------------------------------
    // Calculates this^n (n is a positive integer, doesn't do fractional or negative powers)
    inline VLInt Pow(int n) const
    {
        if (n < 1) return VLInt(1);

        VLUInt val = value.Pow(n);
        bool pstve = positive || (n % 2) == 0;

        return VLInt(val, pstve);
    }
    //------------------------------------------------------------------------------------------------------
    // Convert to f * 10^n (ignores the sign)

    inline std::pair<double,int> MantissaExponent () const
    {
        if (IsZero())
        {
            throw std::exception("zero not supported");
        }

        auto ret = value.MantissaExponent();

        if (!positive)
        {
            ret.first = -ret.first;
        }

        return ret;
    }
    //------------------------------------------------------------------------------------------------------
    // Log base 10
    inline double Log10 () const
    {
        if (IsZero () || ! positive)
        {
            throw std::invalid_argument("Log zero or negative");
        }

        return value.Log10();
    }
    //------------------------------------------------------------------------------------------------------
    inline static double Ratio (const VLInt & b1, const VLInt& b2)
    {
        auto m1 = b1.MantissaExponent();
        auto m2 = b2.MantissaExponent();

        return (m1.first / m2.first) * pow(10, (m1.second - m2.second));
    }
    //------------------------------------------------------------------------------------------------------
    inline VLInt Square () const
    {
        return (*this) * (*this);
    }
    //------------------------------------------------------------------------------------------------------
    inline VLInt Cube () const
    {
        return (*this) * (*this) * (*this);
    }
    //------------------------------------------------------------------------------------------------------
    inline double CubeRoot () const
    {
        auto me = MantissaExponent();

        while (me.second % 3 != 0)
        {
            --me.second;
            me.first *= 10;
        }

        return pow(me.first, 1.0 / 3.0) * pow(10, me.second / 3);
    }
    //------------------------------------------------------------------------------------------------------
    // Don't define operator so that we don't call this by mistake
    inline __int64 ToInt () const
    {
        auto ret = value.ToInt ();

        return (positive) ? ret : -ret;
    }
    //=========================================================================================================
    // Monitoring and Testing
    //=========================================================================================================
    inline std::string ToString () const
    {
        if (IsZero ())
        {
            return "0";
        }

        std::stringstream ret;

        if (!positive)
        {
            ret << '-';
        }

        ret << value;

        return ret.str();
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream & os, const VLInt & vli)
    {
        return os << vli.ToString();
    }
    //------------------------------------------------------------------------------------------------------
    static void Test();

}; // class


