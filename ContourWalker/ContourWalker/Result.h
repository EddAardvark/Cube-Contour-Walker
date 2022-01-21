#pragma once


#include <string>
#include <sstream>
#include <iostream>

#include "VLInt.h"

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

        return sstrm.str().c_str();
    }

    inline std::string toString() const
    {
        std::stringstream sstrm;

        if (flip)
            sstrm << z << "^3 - " << x << "^3 - " << y << "^3 = " << value;
        else
            sstrm << x << "^3 + " << y << "^3 - " << z << "^3 = " << value;

        return sstrm.str().c_str();
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
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const Result& res)
    {
        return os << res.toString();
    }
};
// --------------------------- End of Result -----------------------------------
