#pragma once

#include <map>
#include <list>

#include "VLInt.h"

//-------------------------------------------------------------------------------------------------
// Cube sum searcher results
//
// (c) John Whitehouse 2021
// www.eddaardvark.co.uk
//-------------------------------------------------------------------------------------------------

class WalkingResults
{
    class Result
    {
        VLInt x, y, z;
        bool flip;

    public:

        __int64 value;
        
        Result() {};

        Result(const VLInt& _x, const VLInt& _y, const VLInt& _z, const VLInt& _v)
        {
            flip = !_v.positive;
            value = _v.ToInt();

            if (flip) value = -value;

            x = _x;
            y = _y;
            z = _z;
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
                sstrm << z << "^3 - " << x << "^3 - " << y << " = " << value;
            else
                sstrm << x << "^3 + " << y << "^3 + " << z << " = " << value;

            return sstrm.str().c_str();
        }
        //--------------------------------------------------------------------------------------------
        inline void VerifySolution () const
        {
            auto val = (x.Cube() + y.Cube() - z.Cube()).ToInt ();

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

    std::map<std::string, Result> values;
    std::map<__int64, std::list<std::string>> results;
    __int64 count;

public:

    WalkingResults()
        : count(0)
    {
    }
    //-------------------------------------------------------------------------------------------------
    inline void Add(const Result& result)
    {
        result.VerifySolution();

        auto key = result.Key();

        if (values.find(key) != values.end())
        {
            return;
        }

        values.emplace (key, result);

        results[result.value].emplace_back(key);

        while (results[result.value].size () > 10)
        {
            results[result.value].erase(results[result.value].begin());
        }

        ++ count;
    }
};

