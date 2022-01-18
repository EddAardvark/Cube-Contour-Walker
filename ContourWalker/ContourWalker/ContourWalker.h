#pragma once

#include "ContourPoint.h"
#include "WalkingResults.h"

class ContourWalker
{

    ContourPoint current;
    WalkingResults results;
    VLInt cross;
    __int64 hop;
    __int64 hop_max;

    //--------------------------------------------------------------------------------------------
    void Fill()
    {
        hop = 0;
        hop_max = 0;
        cross = VLInt::FromInt (-1);

        do
        {
            FillNoDraw(0, 500);
        } while (true);
    }

    //--------------------------------------------------------------------------------------------
    void FillNoDraw(int x, int width)
    {
        auto dec = 0;

        while (x < width)
        {
            if (hop >= 2)
            {
                current.HopSub(hop);
                current.IncrementCube();
                hop = 0;

                while (!current.value.positive)
                {
                    current.DecrementSub();
                    ++dec;
                    std::cout << "Overshot, Dec = " << dec << std::endl;
                }
            }

            auto v1 = current.GetNextX();
            auto v2 = v1.GetNextY();
            auto prev = current;

            auto c = VLInt::CompareVector(v1.value.value, v2.value.value);

            current = (c > 0) ? v2 : v1;

        // Check for crossing, positive to negative

            if (prev.value.positive && !current.value.positive)
            {
                if (prev.TestValue())
                {
                    results.Add(prev.GetResult());
                }
                if (current.TestValue())
                {
                    results.Add(current.GetResult());
                }

                auto delta = (cross.positive) ? (prev.subcube.x - cross) : cross;
                cross = prev.subcube.x;
                if (delta.positive)
                {
                    __int64 d = delta.ToInt();
                    if (d > hop_max) hop_max = d;
                }
                hop = hop_max - 2;
            }
            ++x;
        }
    }
};

