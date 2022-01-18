#pragma once

#include <map>
#include <list>

#include "VLInt.h"
#include "Result.h"

//-------------------------------------------------------------------------------------------------
// Cube sum searcher results
//
// (c) John Whitehouse 2021
// www.eddaardvark.co.uk
//-------------------------------------------------------------------------------------------------

class WalkingResults
{
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



