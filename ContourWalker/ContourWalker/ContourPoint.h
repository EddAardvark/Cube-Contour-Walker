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

    static const __int64 target = 1025L;

    BigCube cube;

public:

    SubCube subcube;

    VLInt value;

    inline ContourPoint() {}

    inline ContourPoint(const ContourPoint & other)
        : cube (other.cube)
        , subcube (other.subcube)
        , value (other.value)
    {
        
    }
    //-------------------------------------------------------------------------------------------------
    inline ContourPoint (__int64 contour)
    {
        static double factor = pow(2, 1.0 / 3.0) - 1;

        auto x = VLInt((int)ceil(contour / factor));
        auto y = VLInt(x);
        auto n = VLInt(contour);
        cube = BigCube(y);
        subcube = SubCube(x, n);
        value = cube.value  - subcube.value;
    }
    inline const VLInt& X() const { return subcube.x; }
    inline const VLInt& Y() const { return cube.root; }
    inline const VLInt& Value() const { return value; }
    inline const bool IsPositive() const { return value.positive; }
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
        return value.TestSmall(target);
    }
    //=========================================================================================================
    // Monitoring and Testing
    //=========================================================================================================
    
    inline std::string ToString () const
    {
        std::stringstream sstrm;
        sstrm << "[Contour = " << subcube.n << " (" << subcube.x << "," << cube.root << ") = " << value;

        return sstrm.str();

    }
    //------------------------------------------------------------------------------------------------------
    inline friend std::ostream& operator << (std::ostream& os, const ContourPoint& cp)
    {
        return os << cp.ToString();
    }

}; // class

