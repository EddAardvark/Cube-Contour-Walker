#pragma once


#include <string>
#include <sstream>
#include <iostream>

#include "..\Common\VLInt.h"

class Result
{
    VLInt x, y, z;
    bool flip{ false };

public:

    __int64 value{ 0 };

    Result() {};

    Result(const VLInt& _x, const VLInt& _y, const VLInt& _z, const VLInt& _v)
        : flip(!_v.positive)
        , value(_v.ToInt())
        , x(_x)
        , y(_y)
        , z(_z)
    {
        if (flip) value = -value;
    }

    inline std::string Key() const
    {
        std::stringstream sstrm;
        sstrm << x << "_" << y << "_" << z;

        return sstrm.str();
    }

    inline std::string ToString() const
    {
        std::stringstream sstrm;

        if (flip)
            sstrm << z << "^3 - " << x << "^3 - " << y << "^3 = " << value;
        else
            sstrm << x << "^3 + " << y << "^3 - " << z << "^3 = " << value;

        return sstrm.str();
    }
    //--------------------------------------------------------------------------------------------
    inline void VerifySolution() const
    {
        auto val = (x.Cube() + y.Cube() - z.Cube()).ToInt();

        if (flip)
        {
            val = -val;
        }
        if (value != val)
        {
            std::stringstream sstrm;
            sstrm << "Result: [" << (*this) << "], got " << val;
            throw std::exception(sstrm.str().c_str());
        }
    }
    //--------------------------------------------------------------------------------------------
    inline void ReduceToLowest(__int64 c) const
    {/*
        __int64 n = CommonFactor();

        if (n > 1)
        {
            x = x / n;
            y = y / n;
            z = z / n;
        }
        */
    }
    inline __int64 CommonFactor () const
    {/*
        auto dm1 = x.value.DivMod(c).second;
        auto dm2 = y.value.DivMod(c).second;
        auto dm3 = z.value.DivMod(c).second;

        if (dm1 == 0 && dm2 == 0 && dm3 == 0)
        {
            return c;
        }

        if (dm1 == 0) dm1 = c;
        if (dm2 == 0) dm2 = c;
        if (dm3 == 0) dm3 = c;

        return hcf(hcf(dm1, dm2), dm3);
    }
    inline static __int64 hcf(__int64 n1, __int64 n2)
    {
        if (n1 == n2) return n1;

        if (n1 > n2) n1 = n1 % n2;
        else if (n2 > n1) n2 = n2 % n1;

        return hcf(n1, n2);
        */
        return 1;
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const Result& res)
    {
        return os << res.ToString();
    }
};
// --------------------------- End of Result -----------------------------------
