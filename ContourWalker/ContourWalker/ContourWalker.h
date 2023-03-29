#pragma once

#include <fstream>

#include "ContourPoint.h"
#include "WalkingResults.h"
#include "ContourCommandLine.h"
#include "PolynomialFitter.h"


class ContourWalker
{
    ContourPoint current;
    VLUInt cross;
    PolynomialFitter f1{ 10 };
    PolynomialFitter f2{ 10 };
    PolynomialFitter f3{ 10 };

    __int64 m_iterations;
    __int64 m_chunk_size;
    __int64 m_contour;
    std::string m_file_name;
    bool m_check_polynomials;

    __int64 hop{ 0 };
    __int64 hop_max{ 0 };

public:

    ContourWalker(const ContourCommandLine& cmd)
        : current(cmd.Contour())
        , m_iterations(cmd.Iterations())
        , m_file_name(cmd.FileName())
        , m_chunk_size(cmd.ChunkSize())
        , m_contour(cmd.Contour())
        , m_check_polynomials (cmd.CheckPolynomials())
    {
        std::cout << "Contour = " << current.Contour() << std::endl;

        if (m_iterations == 0)
            std::cout << "Steps = run forever" << std::endl;
        else
            std::cout << "Steps = " << m_iterations << std::endl;

        std::cout << "Chunk Size = " << m_chunk_size << std::endl;
        std::cout << "Output File = " << m_file_name << std::endl;
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

        for (__int64 i = 0; m_iterations == 0 || i < m_iterations; ++i)
        {
            if (m_check_polynomials)
                FillWithCheck(m_chunk_size);
            else
                Fill(m_chunk_size);

            std::cout << "Chunk " << i;
            if (m_iterations > 0) std::cout << ", of " << m_iterations;
            std::cout << ", x = " << current.subcube.x << ", current hop = " << hop_max << std::endl;
        }
    }

protected:

    void Fill(__int64 width)
    {
        for (auto x = 0 ; x < width ; ++x)
        {
            if (hop >= 2)
            {
                current.HopSub(hop);
                hop = 0;
            }

            auto prev = current;

            while (current.IsPositive ())
            {
                prev = current;
                current = current.GetNextX();
            }

            auto v2 = current.GetNextY();

        // Check for crossing, positive to negative

            TestValue(prev);
            TestValue(current);
            TestValue(v2);

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
                    hop_max = d;
                }
            }
            hop = hop_max - 2;
        }
    }



    void FillWithCheck(__int64 width)
    {
        for (auto x = 0; x < width; ++x)
        {
            if (hop >= 2)
            {
                current.HopSub(hop);
                hop = 0;
            }

            auto prev = current;
            int count = 0;

            while (current.IsPositive())
            {
                ++count;
                prev = current;
                current = current.GetNextX();
            }

            auto v2 = current.GetNextY();

            f1.Add(prev.Value());
            f2.Add(current.Value());
            f3.Add(v2.Value());

            TestValue(prev);
            TestValue(current);
            TestValue(v2);

            current = v2;

            auto delta = (cross.IsZero()) ? cross : (current.subcube.x.value - cross);

            cross = current.subcube.x.value;

            if (!delta.IsZero())
            {
                __int64 d = delta.ToInt();

                if (d > hop_max)
                {
                    if (d - hop_max > 1)
                    {
                        std::cout << "Hop max = " << hop_max << ", delta = " << d << std::endl;
                    }
                    hop_max = d;
                }
            }
            hop = hop_max - 2;
        }
    }

    inline void TestValue (const ContourPoint & val)
    {
        if (val.TestValue())
        {
            auto r = val.GetResult();

            //r.CheckDivisors(m_contour);

            std::cout << r << std::endl;
            Write(r);
        }
    }

    void Write(const Result& result)
    {
        std::ofstream file;

        file.open(m_file_name, std::ios_base::app);
        file << result << std::endl;
        file.close();
    }

    void Write(const std::string & text)
    {
        std::ofstream file;

        file.open(m_file_name, std::ios_base::app);
        file << text << std::endl;
        file.close();
    }
};

