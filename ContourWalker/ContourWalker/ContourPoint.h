#pragma once

#include "VLInt.h"
#include "BigCube.h"
#include "SubCube.h"
#include "Result.h"

//-------------------------------------------------------------------------------------------------
// Implements a number of the form y^3 - 3nx^2 + 3n^2x + n^3, equivalent to (x+n)^3 - x^3
//
// Uses VLIntegers to avoid being confined to small numbers
//
// This is a single point on the contour in the map for X^3 + y^3 - z^3 = 0
//
// (c) John Whitehouse 2021, 2022
// www.eddaardvark.co.uk
//-------------------------------------------------------------------------------------------------

class ContourPoint
{
    //-------------------------------------------------------------------------------------------------

    static const __int64 LIMIT = 1025L;

    static VLInt max_target;
    static VLInt min_target;

    VLInt x;
    VLInt y;
    VLInt n;
    BigCube cube;

public:

    SubCube subcube;

    VLInt value;

    inline ContourPoint() {}

    inline ContourPoint(const ContourPoint & other)
    {
        x = other.x;
        y = other.y;
        n = other.n;
        cube = other.cube;
        subcube = other.subcube;
        value = other.value;
    }
    //-------------------------------------------------------------------------------------------------
    inline static ContourPoint FromContour (__int64 contour)
    {
        static double factor = pow(2, 1.0 / 3.0) - 1;

        ContourPoint ret;

        ret.x = VLInt::FromInt((int)ceil(contour / factor));
        ret.y = VLInt::FromVLInt(ret.x);
        ret.n = VLInt::FromInt(contour);
        ret.cube = BigCube::FromVLInt(ret.y);
        ret.subcube = SubCube::FromVLInts(ret.x, ret.n);
        ret.value = ret.cube.value  - ret.subcube.value;

        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline ContourPoint GetNextX () const
    {
        auto ret = (*this);
        ret.IncrementSub();
        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline ContourPoint GetNextY () const
    {
        auto ret = (*this);
        ret.IncrementCube();
        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline ContourPoint GetPreviousX () const
    {
        auto ret = (*this);
        ret.DecrementSub();
        return ret;
    }
    //-------------------------------------------------------------------------------------------------
    inline ContourPoint GetPreviousY () const
    {
        auto ret = (*this);
        ret.DecrementCube();
        return ret;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline void DecrementSub ()
    {
        -- subcube;
        value = value + subcube.dv;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline void IncrementSub ()
    {
        value = value - subcube.dv; // Value = cube - sub, so subtract
        ++ subcube;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline void DecrementCube ()
    {
        -- cube;
        value = value - cube.dy;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline void IncrementCube ()
    {
        value = value + cube.dy;
        ++ cube;
    }
    //----------------------------------------------------------------------------------------------------------------
    inline void HopSub (__int64 hop)
    {
        subcube.Hop(hop);

        value = cube.value  - subcube.value;
    }
    //--------------------------------------------------------------------------------------------
    inline Result GetResult() const
    {
        return Result(subcube.x, cube.root, subcube.x + subcube.n, value);
    }
    //--------------------------------------------------------------------------------------------
    inline bool TestValue () const
    {
        if (value.positive)
        {
            if (VLInt::Compare(value, max_target) <= 0)
            {
                return true;
            }
        }
        else if (VLInt::Compare(value, min_target) >= 0)
        {
            return true;
        }
        return false;
    }
    //=========================================================================================================
    // Monitoring and Testing
    //=========================================================================================================
    
    inline std::string toString () const
    {
        std::stringstream sstrm;
        sstrm << "[CP (" << n << "," << subcube.x << "," << cube.root << ") =" << value << "]";

        return sstrm.str().c_str();
    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const ContourPoint& cp)
    {
        return os << cp.toString();
    }

}; // class

