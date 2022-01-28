#pragma once

#include <fstream>

#include "ContourPoint.h"
#include "WalkingResults.h"
#include "CubicSpotter.h"

class ContourWalker
{
    std::string m_result_file{ "results.txt" };
    ContourPoint current;
    WalkingResults results;
    VLUInt cross;
    CubicSpotter spotter;

    __int64 hop{ 0 };
    __int64 hop_max{ 0 };
    __int64 m_steps{ 1 };
    __int64 m_chunk{ 500 };

public:

    ContourWalker(__int64 contour, __int64 steps, __int64 chunk_size)
        : m_steps(steps)
        , m_chunk(chunk_size)
        , current (contour)
    {
    }

    //--------------------------------------------------------------------------------------------
    void Walk()
    {
        hop = 0;
        hop_max = 0;
        cross = VLUInt(0);

        std::stringstream sstrm;
        sstrm << "Contour starting with " << current;
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
        for (auto x = 0 ; x < width ; ++x)
        {
            if (hop >= 2)
            {
                current.HopSub(hop);
                hop = 0;
            }

            auto prev = current;
            int count = 0;

            while (current.IsPositive ())
            {
                ++count;
                prev = current;
                current = current.GetNextX();
            }

            auto v2 = current.GetNextY();

            /*
            if (hop_max > 3)
            {
                spotter.Add(v1.X(), v1.Value(), v2.Value());
            }

            int test = (prev.IsPositive() ? 4 : 0) + (current.IsPositive() ? 2 : 0) + (v2.IsPositive() ? 1 : 0);

            if (test != 5)
            {
                std::cout << "Count: " << count << ", Test: " << test << ", P: " << prev << ", C: " << current << ", V2: " << v2 << std::endl;
            }
            */

        // Check for crossing, positive to negative

            if (prev.TestValue())
            {
                auto r = prev.GetResult();
                results.Add(r);
                Write(r);
            }
            if (current.TestValue())
            {
                auto r = current.GetResult();

                results.Add(r);
                Write(r);
            }
            if (v2.TestValue())
            {
                auto r = v2.GetResult();
                results.Add(r);
                Write(r);
            }

            current = v2;

            auto delta = (cross.IsZero ()) ? cross : (current.subcube.x.value - cross);

            cross = current.subcube.x.value;

            if (! delta.IsZero ())
            {
                __int64 d = delta.ToInt();

                if (d > hop_max)
                {
                    if (d - hop_max > 1)
                    {
                        std::cout << "Hop max = " << hop_max << ", delta = " << d << std::endl;
                    }
                    spotter.SetDelta(d);
                    hop_max = d;
                }
            }
            hop = hop_max - 2;
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

