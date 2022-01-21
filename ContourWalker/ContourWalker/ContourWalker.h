#pragma once

#include <fstream>

#include "ContourPoint.h"
#include "WalkingResults.h"

class ContourWalker
{
    std::string m_result_file{ "results.txt" };
    ContourPoint current;
    WalkingResults results;
    VLInt cross;
    __int64 hop{ 0 };
    __int64 hop_max{ 0 };
    __int64 m_contour{ 2 };
    __int64 m_steps{ 1 };
    __int64 m_chunk{ 500 };

public:

    ContourWalker(__int64 contour, __int64 steps, __int64 chunk_size)
        : m_contour(contour)
        , m_steps(steps)
        , m_chunk(chunk_size)
    {
    }

    //--------------------------------------------------------------------------------------------
    void Walk()
    {
        hop = 0;
        hop_max = 0;
        cross = VLInt::FromInt (-1);
        current = ContourPoint::FromContour(m_contour);

        std::stringstream sstrm;
        sstrm << "Contour " << m_contour << " starting with " << current;
        Write(sstrm.str());

        for (__int64 i = 0 ; m_steps == 0 || i < m_steps ; ++i)
        {
            FillNoDraw(m_chunk);
            std::cout << "Chunk " << i;
            if (m_steps > 0) std::cout << ", of " << m_steps;
            std::cout << ", x = " << current.subcube.x << std::endl;
        }
    }

protected:

    void FillNoDraw(__int64 width)
    {
        auto dec = 0;

        for (auto x = 0 ; x < width ; ++x)
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
                    Write(prev.GetResult());
                }
                if (current.TestValue())
                {
                    results.Add(current.GetResult());
                    Write(current.GetResult());
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
        }
    }

    void Write(const Result& result)
    {
        std::ofstream file;

        file.open(m_result_file, std::ios_base::app);
        file << result << std::endl;
        file.close();
    }

    void Write(const std::string & text)
    {
        std::ofstream file;

        file.open(m_result_file, std::ios_base::app);
        file << text << std::endl;
        file.close();
    }
};

