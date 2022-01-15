#pragma once

//-------------------------------------------------------------------------------------------------
// Stores very large integers as a set of smaller ones (base 10^N)
// (c) John Whitehouse 2019-2022
// www.eddaardvark.co.uk
//---------------

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <iostream>

class VLInt
{

// MAX_SAFE_INTEGER = 9007199254740991
// Base = biggest power of 10 <= sqrt (MAX)
// I want to work in powers of 10 so I'm doing the maths base 10^n, makes printing easier

    static const int DIGITS = 8; 
    static const int BASE = 100000000; // 10^DIGITS

    static std::vector<VLInt> powers2;

    std::vector<__int64> value;
    bool positive{true};

public:

    //----------------------------------------------------------------------------------------------------------------
    inline static VLInt MakeZero ()
    {
        VLInt ret;

        ret.positive = true;
        ret.value.emplace(ret.value.end(),0);
        return ret;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline static VLInt MakeOne()
    {
        VLInt ret;

        ret.positive = true;
        ret.value.emplace(ret.value.end(), 1);
        return ret;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline static VLInt FromInt (__int64 n)
    {
        VLInt ret;
        ret.positive = (n >= 0);

        if (!ret.positive) n = -n;

        auto i = 0;

        do
        {
            ret.value.emplace(ret.value.end(), n % BASE);
            n = n / BASE;
        } while (n > 0);

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    inline static VLInt FromVLInt (const VLInt & other)
    {
        VLInt ret;

        ret.positive = other.positive;
        ret.value = other.value;
        return ret;
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt & operator = (const VLInt& other)
    {
        if (this != &other)
        {
            positive = other.positive;
            value = other.value;
        }

        return (* this);
    }
    //--------------------------------------------------------------------------------------------
    inline static VLInt FromVector (const std::vector<__int64> & v, bool pos)
    {
        VLInt ret;

        ret.positive = pos;
        ret.value = v;
        return ret;
    }
    //--------------------------------------------------------------------------------------------
    inline static VLInt FromString (const std::string & str)
    {
        VLInt ret;

        ret.positive = true;
        ret.value.emplace(ret.value.end(), 0);

        auto pos = str.size() - 1;

        while (pos > 0)
        {
            if (str[pos] == '-')
            {
                ret.positive = false;
                return ret;
            }

            int n = str[pos] - '0';
            ret = ret * 10 + n;
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Multiply by a simple integer
    //--------------------------------------------------------------------------------------------
    inline VLInt operator * (int num) const
    {
        if (num == 0 || IsZero())
        {
            return MakeZero();
        }

        VLInt ret;

        ret.positive = positive;

        if (num < 0)
        {
            num = -num;
            ret.positive = !ret.positive;
        }

        __int64 carry = 0;
        auto len = value.size();

        for (auto i = 0; i < len; ++i)
        {
            auto v = value[i] * num + carry;
            carry = v / BASE;
            ret.value.emplace(ret.value.end(), v % BASE);
        }

        while (carry > 0)
        {
            ret.value.emplace(ret.value.end(), carry % BASE);
            carry = carry / BASE;
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Multiply by a another big integer
    //--------------------------------------------------------------------------------------------
    inline VLInt operator * (const VLInt & other) const
    {
        if (other.IsZero() || IsZero())
        {
            return MakeZero();
        }

        VLInt ret;

        ret.positive = positive == other.positive;
        ret.value = MultiplyVectors(value, other.value);

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Add a simple number (this number is truncated to an integer)
    // TODO: Optimise using values directly
    //--------------------------------------------------------------------------------------------
    inline VLInt operator + (__int64 num) const
    {
        auto n = FromInt(num);

        return (*this) + n;
    }
    //--------------------------------------------------------------------------------------------
    // Add a simple number (this number is truncated to an integer)
    // TODO: Optimise using values directly
    //--------------------------------------------------------------------------------------------
    inline VLInt operator - (__int64 num) const
    {
        auto n = FromInt(num);

        return (*this) - n;
    }
    //--------------------------------------------------------------------------------------------
    // Pre increment
    //--------------------------------------------------------------------------------------------
    inline VLInt & operator ++ ()
    {
        if (IsZero())
        {
            value.clear();
            positive = true;
            value.emplace(value.end(), 1);
            return (*this);
        }

        if (positive)
        {
            IncrementVector();
            return (*this);
        }

        DecrementVector();
        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post increment
    //--------------------------------------------------------------------------------------------
    inline VLInt& operator ++ (int)
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
            value.clear();
            positive = false;
            value.emplace(value.end(), 1);
            return (*this);
        }

        if (positive)
        {
            DecrementVector();
            return (*this);
        }

        IncrementVector();
        return (*this);
    }
    //--------------------------------------------------------------------------------------------
    // Post decrement
    //--------------------------------------------------------------------------------------------
    inline VLInt& operator -- (int)
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
        VLInt ret;

        if (other.positive == positive)
        {
            ret.positive = positive;
            ret.value = AddVectors(value, other.value);
            return ret;
        }

        auto comp = CompareVector(value, other.value);

        if (comp == 0)
        {
            ret.positive = true;
            ret.value.emplace(ret.value.end(), 0);
            return ret;
        }

        if (comp > 0)
        {
            ret.value = SubtractVectors(value, other.value);
            ret.positive = positive;
            return ret;
        }

        ret.value = SubtractVectors(other.value, value);
        ret.positive = other.positive;
        return ret;
    }
    //--------------------------------------------------------------------------------------------
    // Subtract
    //--------------------------------------------------------------------------------------------
    inline VLInt operator - (const VLInt& other) const
    {
        VLInt ret;

        if (other.positive != positive)
        {
            ret.positive = positive;
            ret.value = AddVectors(value, other.value);
            return ret;
        }

        auto comp = CompareVector(value, other.value);

        if (comp == 0)
        {
            ret.positive = true;
            ret.value.emplace(ret.value.end(), 0);
            return ret;
        }

        if (comp > 0)
        {
            ret.value = SubtractVectors(value, other.value);
            ret.positive = positive;
            return ret;
        }

        ret.value = SubtractVectors(other.value, value);
        ret.positive = !other.positive;
        return ret;
    }
    //--------------------------------------------------------------------------------------------
    inline static std::vector<__int64> AddVectors (const std::vector<__int64> & vec1, const std::vector<__int64> & vec2)
    {
        auto len1 = vec1.size();
        auto len2 = vec2.size();
        auto len = std::max(len1, len2);
        std::vector<__int64> ret;
        __int64 carry = 0;

        for (auto i = 0; i < len; ++i)
        {
            __int64 sum = carry;
            if (i < len1) sum += vec1[i];
            if (i < len2) sum += vec2[i];

            ret.emplace(ret.end(), sum % BASE);
            carry = sum / BASE;
        }

        if (carry > 0)
        {
            ret.emplace(ret.end(), carry);
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    inline void IncrementVector ()
    {
        auto n = value.size() - 1;
        ++value[0];

        for (auto i = 0; i < n && value[i] == BASE; ++i)
        {
            value[i] = 0;
            ++value[i + 1];
        }

        if (value[n] == BASE)
        {
            value[n] = 0;
            value.emplace(value.end(), 1);
        }
    }
    //--------------------------------------------------------------------------------------------
    inline void DecrementVector ()
    {
        auto n = value.size() - 1;
        --value[0];

        for (auto i = 0; i < n && value[i] < 0; ++i)
        {
            value[i] = BASE - 1;
            --value[i + 1];
        }

        if (value[n] < 0)
        {
            throw std::logic_error("Decrement past 0 not supported");
        }
    }
    //--------------------------------------------------------------------------------------------
    inline static std::vector<__int64> SubtractVectors (const std::vector<__int64> & vec1, const std::vector<__int64> & vec2)
    {
        // Expects vec1 >= vec2

        auto len = vec1.size();
        std::vector<__int64> ret;
        __int64 carry = 0;

        for (auto i = 0; i < len; ++i)
        {
            auto diff = carry;
            if (i < vec1.size()) diff += vec1[i];
            if (i < vec2.size()) diff -= vec2[i];

            if (diff < 0)
            {
                diff += BASE;
                carry = -1;
            }
            else
            {
                carry = 0;
            }
            ret.emplace (ret.end (), diff);
        }

        if (carry < 0)
        {
            throw std::logic_error("Negative carry");
        }

        while (ret.size() > 0 && ret[ret.size()-1] == 0)
        {
            ret.erase (ret.end()-1);
        }

        return ret;
    }
    //--------------------------------------------------------------------------------------------
    inline static std::vector<__int64> MultiplyVectors (const std::vector<__int64> &vec1, const std::vector<__int64> &vec2)
    {
        auto len1 = vec1.size();
        auto len2 = vec2.size();
        auto len = len1 + len2;
        std::vector<__int64> ret;

        for (auto i = 0; i < len; ++i)
        {
            ret.emplace(ret.end(), 0);
        }

        for (auto i = 0; i < len1; ++i)
        {
            auto v1 = vec1[i];

            for (auto j = 0; j < len2; ++j)
            {
                auto v2 = v1 * vec2[j];
                auto pos = i + j;
                ret[pos] += v2;

                while (ret[pos] > BASE)
                {
                    ret[pos + 1] += ret[pos] / BASE;
                    ret[pos] %= BASE;
                    ++pos;
                }
            }
        }

        while (ret.size() > 1 && ret[ret.size() - 1] == 0)
        {
            ret.erase (ret.end()-1);
        }

        return ret;
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

        auto c = VLInt::CompareVector(first.value, second.value);

        return first.positive ? c : -c;
    }
    //--------------------------------------------------------------------------------------------
    // returns the minimum (by reference)
    //--------------------------------------------------------------------------------------------
    inline static VLInt Min (const VLInt& first, const VLInt& second)
    {
        return (VLInt::Compare(first, second) > 0) ? second : first;
    }
    //--------------------------------------------------------------------------------------------
    // returns the maximum (by reference)
    //--------------------------------------------------------------------------------------------
    inline static VLInt Max(const VLInt& first, const VLInt& second)
    {
        return (VLInt::Compare(first, second) > 0) ? first : second;
    }
    //--------------------------------------------------------------------------------------------
    // Compare the vector component ignoring the positive,
    // returns -1, 0 or 1 for (first < second), (first == second) and (first > second)
    // Assumes the vector have no leading 0's
    //--------------------------------------------------------------------------------------------
    inline static int CompareVector(const std::vector<__int64>& first, const std::vector<__int64>& second)
    {
        if (first.size() != second.size())
        {
            return (first.size() > second.size()) ? 1 : -1;
        }
        if (first.size() == 0)
        {
            return 0;
        }
        int pos = (int)(first.size() - 1);

        while (pos >= 0)
        {
            if (first[pos] != second[pos])
            {
                return (first[pos] > second[pos]) ? 1 : -1;
            }
            --pos;
        }
        return 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator > (const VLInt& other) const
    {
        return VLInt::Compare((*this), other) > 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator >= (const VLInt& other) const
    {
        return VLInt::Compare((*this), other) >= 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator < (const VLInt& other) const
    {
        return VLInt::Compare((*this), other) < 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator <= (const VLInt& other) const
    {
        return VLInt::Compare((*this), other) <= 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator == (const VLInt& other) const
    {
        return VLInt::Compare((*this), other) == 0;
    }
    //--------------------------------------------------------------------------------------------
    inline bool operator != (const VLInt& other) const
    {
        return VLInt::Compare((*this), other) != 0;
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt Abs () const
    {
        return VLInt::FromVector(value, true);
    }
    //--------------------------------------------------------------------------------------------
    inline VLInt operator - () const
    {
        return VLInt::FromVector(value, !positive);
    }
    //--------------------------------------------------------------------------------------------
    inline bool IsZero () const
    {
        return value.size() == 0 || (value.size() == 1 && value[0] == 0);
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod2 () const
    {
        return (value.size() > 0) ? (value[0] % 2) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod3() const
    {
        if (IsZero())
        {
            return 0;
        }

        auto sum = value[0];

        for (auto i = 1; i < value.size(); ++i)
        {
            sum += value[i];
        }
        return sum % 3;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod4() const
    {
        return (value.size() > 0) ? (value[0] % 4) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod5() const
    {
        return (value.size() > 0) ? (value[0] % 5) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod8() const
    {
        return (value.size() > 0) ? (value[0] % 8) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod9() const
    {
        if (IsZero())
        {
            return 0;
        }

        auto sum = value[0];

        for (auto i = 1; i < value.size(); ++i)
        {
            sum += value[i];
        }
        return sum % 9;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod10() const
    {
        return (value.size() > 0) ? (value[0] % 10) : 0;
    }
    //--------------------------------------------------------------------------------------------
    inline int Mod16() const
    {
        return (value.size() > 0) ? (value[0] % 16) : 0;
    }
    //--------------------------------------------------------------------------------------------
    // Divide by an integer (< BASE)
    inline VLInt DivideByInt (__int64 number)
    {
        return DivMod(number).first;
    }
    inline __int64 Remainder (__int64 number)
    {
        return DivMod(number).second;
    }

    inline std::pair<VLInt, __int64> DivMod (__int64 number)
    {
        std::pair<VLInt, __int64> ret;

        if (number <= 0 || number > BASE)
        {
            return ret;
        }
        auto len = (int) value.size();
        std::vector<__int64> val;
        auto buffer = value;
        __int64 rem = 0;

        for (int i = len - 1; i >= 0; --i)
        {
            rem = buffer[i] % number;

            buffer[i] = buffer[i] / number;

            if (i > 0)
            {
                buffer[i - 1] += BASE * rem;
            }
        }

        ret.first = VLInt::FromVector(buffer, positive);
        ret.second = rem;
        return ret;
    }
    // Divide, this/other, returns a VLInt

    inline VLInt operator / (const VLInt & other)
    {
        if (other.IsZero())
        {
            throw std::invalid_argument("Divide by zero");
        }

        if (VLInt::CompareVector(other.value, value) > 0)
        {
            return VLInt::MakeZero();
        }

        auto pstv = positive == other.positive;
        auto num = VLInt::FromVLInt(*this);
        auto den = VLInt::FromVLInt(other);

        num.positive = true;
        den.positive = true;

        std::vector <VLInt> subs;
        auto answer = VLInt::MakeZero ();
        auto pos = 0;

        subs.emplace (subs.end(), den);

        // We will subtract out powers of 2 times the divisor

        while (num > subs[pos])
        {
            subs.emplace(subs.end(), subs[pos] * 2);        // denominator x 2^n
            ++pos;
        }

        VLInt::PreparePowers(subs.size());

        while (pos >= 0)
        {
            if (num >= subs[pos])
            {
                answer = answer + powers2[pos];
                num = num - subs[pos];
            }
            --pos;
        }

        if (answer.IsZero())
        {
            answer.positive = true;
        }
        else if (!pstv)
        {
            answer.positive = false;
        }
        return answer;
    }

    // Saves recalculating these every time we do a division

    inline static void PreparePowers (size_t n)
    {
        auto len = powers2.size();

        if (n > len)
        {
            for (auto i = powers2.size(); i < n; ++i)
            {
                powers2.emplace (powers2.end(), powers2[i - 1] * 2);    // 2^n
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    // Calculates this^n (n is a positive integer, doesn't do fractional or negative powers)

    inline VLInt Pow (int n)
    {
        if (n < 1) return VLInt::MakeOne();

        std::vector<VLInt> bits;
        auto p = (*this);

        while (true)
        {
            if (n % 2 == 1)
            {
                bits.emplace (bits.end(),p);
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

    inline std::pair<double,int> MantissaExponent () const
    {
        if (IsZero())
        {
            return std::make_pair(0,1);
        }

        auto len = value.size();
        auto exponent = DIGITS * len;
        double mantissa = (double)value[len - 1] / BASE;

        if (len > 1)
        {
            mantissa += (double) value[len - 2] / BASE / BASE;

            if (len > 2)
            {
                mantissa += (double) value[len - 3] / BASE / BASE / BASE;
            }
        }

        while (mantissa < 1)
        {
            mantissa *= 10;
            exponent--;
        }

        return std::make_pair(mantissa, (int) exponent);
    }
    //------------------------------------------------------------------------------------------------------
    // Log base 10
    inline double Log10 () const
    {
        auto me = MantissaExponent();

        if (me.first == 0)
        {
            throw std::invalid_argument("Log 0");
        }

        return me.second + log10(me.first);
    }
    //------------------------------------------------------------------------------------------------------
    inline static double Ratio (const VLInt & b1, const VLInt& b2)
    {
        auto m1 = b1.MantissaExponent();
        auto m2 = b2.MantissaExponent();

        return (m1.first / m2.first) * pow(10, (m1.second - m2.second));
    }
    //------------------------------------------------------------------------------------------------------
    inline VLInt Square ()
    {
        return (*this) * (*this);
    }
    //------------------------------------------------------------------------------------------------------
    inline VLInt Cube ()
    {
        return (*this) * (*this) * (*this);
    }
    //------------------------------------------------------------------------------------------------------
    inline double CubeRoot ()
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
    inline __int64 ToInt ()
    {
        auto len = value.size();

        if (len < 1) return 0;

        auto ret = value[0];

        for (auto i = 1; i < len; ++i)
        {
            double f = ret * (double) BASE + value[i];

            if (f > LONG_MAX)
            {
                throw std::invalid_argument("Overflow");
            }

            ret = ret * BASE + value[i];
        }

        return (positive) ? ret : -ret;
    }
    //=========================================================================================================
    // Monitoring and debugging
    //=========================================================================================================
    inline std::string toString () const
    {
        int len = (int) value.size();

        if (len == 0)
        {
            return "0";
        }

        std::stringstream ret;

        if (!positive)
        {
            ret << '-';
        }

        ret << value[len-1] << std::setfill('0');

        for (auto i = len-2 ; i >= 0 ; --i)
        {
            ret << std::setw(DIGITS) << value[i];
        }
        return ret.str();
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream & os, const VLInt & vli)
    {
        return os << vli.toString();
    }
    //------------------------------------------------------------------------------------------------------
    inline static void Test ()
    {
        // AddInt

        __int64 to_add[11] = { -5, -1, 0, 1, 5, 9, 10, 12, 24, 33, 111 };
        __int64 start[3] = { -99, 0, 99 };

        // Basic Addition

        for (auto i = 0; i < 3; ++i)
        {
            __int64 x = start[i];
            auto vlx = VLInt::FromInt(start[i]);

            if (vlx.ToInt() != x)
            {
                std::ostringstream sstrm;
                
                sstrm << "Initialise vlx " << start[i] << " returned " << vlx;
                
                throw std::exception(sstrm.str().c_str());
            }

            for (auto j = 0; j < 11; ++j)
            {
                auto was = x;
                x += to_add[j];
                vlx = vlx + to_add[j];

                if (vlx.ToInt() != x)
                {
                    std::ostringstream sstrm;

                    sstrm << "Add: " << was << " + " << to_add[j] << ", got " << vlx << ", expected " << x;

                    throw std::exception(sstrm.str().c_str());
                }
            }

        }

        // Basic Subtraction

        for (auto i = 0; i < 3; ++i)
        {
            __int64 x = start[i];
            auto vlx = VLInt::FromInt(start[i]);

            for (auto j = 0; j < 11; ++j)
            {
                auto was = x;
                x -= to_add[j];
                vlx = vlx - to_add[j];

                if (vlx.ToInt() != x)
                {
                    std::ostringstream sstrm;

                    sstrm << "Add: " << was << " - " << to_add[j] << ", got " << vlx << ", expected " << x;

                    throw std::exception(sstrm.str().c_str());
                }
            }
        }

        // Factorials (multiply by int)

        std::string f40 = "815915283247897734345611269596115894272000000000";

        VLInt fn[41];
        
        fn[0] = VLInt::FromInt(1);

        for (int i = 1; i <= 40; ++i)
        {
            fn [i] = fn [i-1] * i;
        }

        if (fn[40].toString() != f40)
        {
            throw std::exception("Factorial 40");
        }

        // Factorials (multiply by VLInt)

        VLInt fact = VLInt::FromInt(1);

        for (int i = 1; i <= 40; ++i)
        {
            VLInt vli = VLInt::FromInt(i);
            fact = fact * vli;

            if (fact != fn[i])
            {
                std::stringstream sstrm;
                sstrm << "Factorial [" << i << "] " << fact << " != " << fn[i];
                throw std::exception(sstrm.str().c_str());
            }
        }

        // Division by VLInt

        for (int i = 0; i < 40; ++i)
        {
            int n = 40 - i;
            VLInt vli = VLInt::FromInt(n);

            fact = fact / vli;

            if (fact != fn[n-1])
            {
                std::stringstream sstrm;
                sstrm << "Factorial Division [" << i << "] " << fact << " != " << fn[n-1];
                throw std::exception(sstrm.str().c_str());
            }
        }

        // Cubes, Mod 9, Mod 3 + increment

        auto vli = FromInt(1000000);
        static int mod9[3] = { 0, 1, 8 };

        for (int i = 0; i < 40; ++i)
        {
            auto c = vli.Cube();
            auto m9 = c.Mod9();
            auto m3 = vli.Mod3();

            if (m9 != mod9[m3])
            {
                std::stringstream sstrm;
                sstrm << "Modulo test: " << vli << " cubed = " << c << ", mod 9 = " << m9 << ", mod 3 = " << m3 << std::endl;
                throw std::exception(sstrm.str().c_str());
            }

            ++vli;
        }

        // Decrement

        for (int i = 0; i < 40; ++i)
        {
            --vli;
        }

        if (vli.ToInt() != 1000000)
        {
            std::stringstream sstrm;
            sstrm << "Decrement test: " << vli << " !- " << 1000000 << std::endl;
            throw std::exception(sstrm.str().c_str());
        }

        // Log 10

        vli = FromInt(2);
        auto log2 = log10(2.0);
        auto check = (int)((log2 - (int)round(log2)) * 100000000);

        for (int i = 0; i < 40; ++i)
        {
            double l2 = vli.Log10();
            int ipart = (int)round(l2);
            double fpart = l2 - ipart;
            int ifpart = (int)(fpart * 100000000);

            if (ifpart != check || ipart != i)
            {
                std::stringstream sstrm;
                sstrm << "Power test: " << "Log (" << vli << ") = " << l2 << ": expected " << ipart << " == " << i << " and " << ifpart << " == " << check << std::endl;
                throw std::exception(sstrm.str().c_str());
            }
            vli = vli * 10;
        }

        // Ratio & Pow, should give 1.099511627776

        auto two = FromInt(2);
        auto ten = FromInt(10);
        auto rcheck = 99511627776L;

        ten = ten.Pow(12);
        two = two.Pow(40);

        auto rat = Ratio(two, ten);
        __int64 rchk2 = (__int64)((rat - 1) * 1000000000000);

        if (rcheck != rchk2)
        {
            std::stringstream sstrm;
            sstrm << "Ratio test: " << two << " / " << ten << " = " << rat << " (" << rcheck << " != " << rchk2 << ") " << std::endl;
            throw std::exception(sstrm.str().c_str());
        }

        // Finished

        std::cout << "VLInt: All tests passed." << std::endl;
    }



}; // class


