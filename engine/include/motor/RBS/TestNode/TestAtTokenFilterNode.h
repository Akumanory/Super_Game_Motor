#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include <motor/RBS/Structure/Condition.h>

class TestAtTokenFilterNode {
public:
    using JudgeFunctionType = std::function<bool(const std::string&, const std::string&)>;

    bool PerformTest(const Condition& c);

    bool IsNeedFilterToken(const Condition& c);

    void InsertJudgeFunction(const std::string& key, JudgeFunctionType& value);

private:

    std::unordered_map<std::string, JudgeFunctionType> dict;
};