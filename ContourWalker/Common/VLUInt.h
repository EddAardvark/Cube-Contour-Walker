#pragma once

#include <vector>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <sstream>

class VLUInt
{
    // Base = 100 million, I want to work in powers of 10 so I'm doing the maths base 10^n, makes printing and debugging easier

    static const int DIGITS = 8;
    //static const int MAX_LEN = 60;  // 10^480
    static const int MAX_LEN = 15;  // 10^120, cube root = 10^40
    static const __int64 BASE = 100000000; // 10^DIGITS

    static std::vector<VLUInt> powers2;

    __int64 value[MAX_LEN]{ 0 };    // [0] is the least significant digit
    int length{ 0 };

public:

    // TODO make this debug only

    inline static void CheckNegative(__int64 n)
    {
        if (n < 0)
        {
            throw std::exception("negative argument for unsigned not supported");
        }
    }

    //----------------------------------------------------------------------------------------------------------------
    inline VLUInt()
    {
    }
    //----------------------------------------------------------------------------------------------------------------
    inline VLUInt (__int64 n)
    {
        length = 0;

        CheckNegative(n);

        do
        {
            value[length] = n % BASE;
            length++;
            n = n / BASE;
        } while (n > 0);
    }
    //--------------------------------------------------------------------------------------------
    inline VLUInt (const VLUInt& other)
        : length (other.length)
    {
        ::memcpy(value, other.value, sizeof(value));
    }
    //--------------------------------------------------------------------------------------------
    inline bool TestSmall(int target) const
    {
        return length == 1 && value [0] < target;
    }
    //--------------------------------------------------------------------------------------------
    inline VLUInt& operator = (const VLUInt& other)
    {
        if (this != &other)
        {
            ::memcpy(value, other.value, sizeof(value));
            length = other.length;
        }

        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    inline VLUInt& operator = (__int64 n)
    {
        length = 0;

        CheckNegative(n);

        do
        {
            value[length] = n % BASE;
            length++;
            n = n / BASE;
        } while (n > 0);

        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    inline bool IsZero() const { return length == 0 || (length == 1 && value[0] == 0); }
    //--------------------------------------------------------------------------------------------
    // Multiply by a simple integer
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator * (__int64 num) const
    {
        if (num == 0 || IsZero())
        {
            return VLUInt(0);
        }

        CheckNegative(num);

        VLUInt ret; 
        __int64 carry = 0;

        ret.length = length;

        for (auto i = 0; i < ret.length; ++i)
        {
            auto v = value[i] * num + carry;
            carry = v / BASE;
            ret.value[i] = v % BASE;
        }

        while (carry > 0)
        {
            ret.value[ret.length] = carry % BASE;
            ret.length++;
            carry = carry / BASE;
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Multiply by a another big integer
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator * (const VLUInt& other) const
    {
        if (other.IsZero() || IsZero())
        {
            return VLUInt(0);
        }

        VLUInt ret;
        auto len1 = length;
        auto len2 = other.length;
        auto len = len1 + len2;

        for (auto i = 0; i < len1; ++i)
        {
            auto v1 = value[i];

            for (auto j = 0; j < len2; ++j)
            {
                auto v2 = v1 * other.value[j];
                auto pos = i + j;
                ret.value[pos] += v2;
            }
        }

        for (int i = 0 ; i < len-1 ; ++i)
        {
            ret.value[i + 1] += ret.value[i] / BASE;
            ret.value[i] %= BASE;
        }

        if (ret.value[len - 1] > 0)
        {
            ++len;
        }

        ret.length = len;

        while (ret.length > 0 && ret.value[ret.length-1] == 0)
        {
            --ret.length;
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Add a simple number (this number is truncated to an integer)
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator + (__int64 num) const
    {
        CheckNegative(num);

        VLUInt ret;

        ret.value[0] = value[0] + num;
        auto carry = ret.value[0] / BASE;
        ret.value[0] %= BASE;
        ret.length = length;

        for (int i = 1; i < length; ++i)
        {
            if (carry > 0)
            {
                ret.value[i] = value [i] + carry;
                carry = ret.value[i] / BASE;
                ret.value[i] %= BASE;
            }
            else
            {
                ret.value[i] = value[i];
            }
        }

        while (carry > 0)
        {
            ret.value[ret.length] = carry % BASE;
            carry /= BASE;
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Subtract a simple number
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator - (__int64 num) const
    {
        CheckNegative(num);

        VLUInt n(num);

        return (*this) - n;
    }
    //--------------------------------------------------------------------------------------------
    // Pre increment
    //--------------------------------------------------------------------------------------------
    inline VLUInt& operator ++ ()
    {
        if (IsZero())
        {
            value[0] = 1;
            length = 1;
            return (*this);
        }

        ++value[0];

        for (auto i = 0; i < (length-1) && value[i] == BASE; ++i)
        {
            value[i] = 0;
            ++value[i + 1];
        }

        if (value[length-1] == BASE)
        {
            value[length - 1] = 0;
            value[length] = 1;
            length++;
        }
        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post increment
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator ++ (int)
    {
        VLUInt temp (*this);

        ++(*this);
        return temp;
    }
    //--------------------------------------------------------------------------------------------
    // Pre decrement
    //--------------------------------------------------------------------------------------------
    inline VLUInt & operator -- ()
    {
        if (IsZero())
        {
            throw std::exception("Can't decrement zero");
        }

        --value[0];

        for (auto i = 0; i < (length-1) && value[i] < 0; ++i)
        {
            value[i] = BASE - 1;
            --value[i + 1];
        }

        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post decrement
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator -- (int)
    {
        VLUInt temp (* this);

        --(*this);
        return temp;
    }
    //--------------------------------------------------------------------------------------------
    // Add
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator + (const VLUInt& other) const
    {
        VLUInt ret;

        auto len1 = length;
        auto len2 = other.length;
        auto len = std::max(len1, len2);

        __int64 carry = 0;

        for (auto i = 0; i < len; ++i)
        {
            __int64 sum = carry;

            if (i < len1) sum += value[i];
            if (i < len2) sum += other.value[i];

            ret.value[i] = sum % BASE;
            carry = sum / BASE;
        }

        ret.length = len;

        if (carry > 0)
        {
            ret.value[ret.length] = carry;
            ++ret.length;
        }
        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Subtract
    //--------------------------------------------------------------------------------------------
    inline VLUInt operator - (const VLUInt& other) const
    {
        VLUInt ret;

        // Expects vec1 >= vec2

        if (other.length > length)
        {
            throw std::exception("Subtraction would result in negative result");
        }

        __int64 carry = 0;
        int len = 0;

        ret.length = length;

        for (int i = 0; i < length ; ++i)
        {
            auto sub = (i < other.length) ? other.value[i] : 0;
            auto diff = carry + value [i] - sub;

            if (diff < 0)
            {
                diff += BASE;
                carry = -1;
            }
            else
            {
                carry = 0;
            }
            ret.value [i] = diff;
        }

        if (carry < 0)
        {
            throw std::exception("Subtraction would result in negative result");
        }
        
        while (ret.length > 0 && ret.value[ret.length - 1] == 0)
        {
            --ret.length;
        }
        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Compare, returns -1, 0 or 1 for (first < second), (first == second) and (first > second)
    //--------------------------------------------------------------------------------------------
    inline static int Compare(const VLUInt& first, const VLUInt& second)
    {
        if (first.length != second.length)
        {
            return (first.length > second.length) ? 1 : -1;
        }

        for (int i = first.length - 1; i >= 0; --i)
        {
            if (first.value[i] != second.value[i])
            {
                return (first.value[i] > second.value[i]) ? 1 : -1;
            }
        }

        return 0;     
    }
    //--------------------------------------------------------------------------------------------
    // returns the minimum (by reference)
    //--------------------------------------------------------------------------------------------
    inline static const VLUInt & Min(const VLUInt& first, const VLUInt& second)
    {
        return (Compare(first, second) > 0) ? second : first;
    }
    //--------------------------------------------------------------------------------------------
    // returns the maximum (by reference)
    //--------------------------------------------------------------------------------------------
    inline static const VLUInt& Max(const VLUInt& first, const VLUInt& second)
    {
        return (Compare(first, second) > 0) ? first : second;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator > (const VLUInt& other) const
    {
        return Compare((*this), other) > 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator >= (const VLUInt& other) const
    {
        return Compare((*this), other) >= 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator < (const VLUInt& other) const
    {
        return Compare((*this), other) < 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator <= (const VLUInt& other) const
    {
        return Compare((*this), other) <= 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator == (const VLUInt& other) const
    {
        return Compare((*this), other) == 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator != (const VLUInt& other) const
    {
        return Compare((*this), other) != 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod2() const
    {
        return (length > 0) ? (value[0] % 2) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod3() const
    {
        if (length == 0)
        {
            return 0;
        }

        auto sum = value[0];

        for (auto i = 1; i < length; ++i)
        {
            sum += value[i];
        }
        return sum % 3;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod4() const
    {
        return (length > 0) ? (value[0] % 4) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod5() const
    {
        return (length > 0) ? (value[0] % 5) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod8() const
    {
        return (length > 0) ? (value[0] % 8) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod9() const
    {
        if (length == 0)
        {
            return 0;
        }

        auto sum = value[0];

        for (auto i = 1; i < length; ++i)
        {
            sum += value[i];
        }
        return sum % 9;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod10() const
    {
        return (length > 0) ? (value[0] % 10) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod16() const
    {
        return (length > 0) ? (value[0] % 16) : 0;
    }
    //--------------------------------------------------------------------------------------------
    // Divide by an integer (< BASE)
    inline VLUInt operator / (__int64 number) const
    {
        return DivMod(number).first;
    }
    inline __int64 operator % (__int64 number)
    {
        return DivMod(number).second;
    }

    inline std::pair<VLUInt, __int64> DivMod(__int64 number) const
    {
        std::pair<VLUInt, __int64> ret;

        if (number <= 0 || number > BASE)
        {
            return ret;
        }
        
        VLUInt temp = (*this);

        __int64 rem = 0;

        for (int i = length - 1; i >= 0; --i)
        {
            rem = temp.value[i] % number;

            temp.value[i] = temp.value[i] / number;

            if (i > 0)
            {
                temp.value[i - 1] += BASE * rem;
            }
        }

        ret.first = temp;
        ret.second = rem;
        return ret;
    }
    // Divide, this/other, returns a VLUInt

    inline VLUInt operator / (const VLUInt& other) const
    {
        if (other.IsZero())
        {
            throw std::invalid_argument("Divide by zero");
        }

        if (other > (*this))
        {
            return VLUInt(0);
        }

        auto num = (*this);
        auto den = other;

        std::vector <VLUInt> subs;
        auto answer = VLUInt (0);
        auto pos = 0;

        subs.emplace(subs.end(), den);

        // We will subtract out powers of 2 times the divisor

        while (num > subs[pos])
        {
            subs.emplace(subs.end(), subs[pos] * 2);        // denominator x 2^n
            ++pos;
        }

        PreparePowers(subs.size());

        while (pos >= 0)
        {
            if (num >= subs[pos])
            {
                answer = answer + powers2[pos];
                num = num - subs[pos];
            }
            --pos;
        }

        return answer;
    }

    // Saves recalculating these every time we do a division

    inline static void PreparePowers(size_t n)
    {
        auto len = powers2.size();

        if (n > len)
        {
            for (auto i = powers2.size(); i < n; ++i)
            {
                powers2.emplace(powers2.end(), powers2[i - 1] * 2);    // 2^n
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    // Calculates this^n (n is a positive integer, doesn't do fractional or negative powers)

    inline VLUInt Pow(int n) const
    {
        if (n < 1) return VLUInt(1);

        std::vector<VLUInt> bits;
        auto p = (*this);

        while (true)
        {
            if (n % 2 == 1)
            {
                bits.emplace(bits.end(), p);
            }
            n = n / 2;

            if (n < 1) break;

            p = p * p;
        }

        auto ret = bits[0];

        for (auto i = 1; i < bits.size(); ++i)
        {
            ret = ret * bits[i];
        }

        return ret;
    }
    //------------------------------------------------------------------------------------------------------
    // Convert to f * 10^n (ignores the sign)

    inline std::pair<double, int> MantissaExponent() const
    {
        if (IsZero())
        {
            throw std::invalid_argument("Log 0");
        }

        auto exponent = DIGITS * length;
        double mantissa = (double)value[length - 1] / BASE;

        if (length > 1)
        {
            mantissa += (double)value[length - 2] / BASE / BASE;

            if (length > 2)
            {
                mantissa += (double)value[length - 3] / BASE / BASE / BASE;
            }
        }

        while (mantissa < 1)
        {
            mantissa *= 10;
            exponent--;
        }

        return std::make_pair(mantissa, (int)exponent);
    }
    //------------------------------------------------------------------------------------------------------
    // Log base 10
    inline double Log10() const
    {
        auto me = MantissaExponent();

        return me.second + log10(me.first);
    }
    //------------------------------------------------------------------------------------------------------
    inline static double Ratio(const VLUInt& b1, const VLUInt& b2)
    {
        auto m1 = b1.MantissaExponent();
        auto m2 = b2.MantissaExponent();

        return (m1.first / m2.first) * pow(10, (m1.second - m2.second));
    }
    //------------------------------------------------------------------------------------------------------
    inline VLUInt Square() const
    {
        return (*this) * (*this);
    }
    //------------------------------------------------------------------------------------------------------
    inline VLUInt Cube() const
    {
        return (*this) * (*this) * (*this);
    }
    //------------------------------------------------------------------------------------------------------
    inline double CubeRoot() const
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
    inline __int64 ToInt() const
    {
        if (length == 0) return 0;

        auto ret = value[0];

        for (auto i = 1; i < length; ++i)
        {
            double f = ret * (double)BASE + value[i];

            if (f > LONG_MAX)
            {
                throw std::invalid_argument("Overflow");
            }

            ret = ret * BASE + value[i];
        }

        return ret;
    }
    //=========================================================================================================
    // Monitoring and Testing
    //=========================================================================================================
    inline std::string ToString() const
    {
        if (length == 0)
        {
            return "0";
        }

        std::stringstream ret;

        ret << value[length - 1] << std::setfill('0');

        for (auto i = length - 2; i >= 0; --i)
        {
            ret << std::setw(DIGITS) << value[i];
        }
        return ret.str();
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const VLUInt& vli)
    {
        return os << vli.ToString();
    }
    //------------------------------------------------------------------------------------------------------
    
    static void Test();

};

