#pragma once

#include <unordered_map>
#include <unordered_set>

#include <motor/RBS/TestNode/TestAtTokenFilterNode.h>
#include <motor/RBS/Structure/Condition.h>
#include <motor/RBS/Alpha/AlphaMemory.h>
#include <motor/RBS/Beta/ProductionNode.h>
#include <motor/RBS/TestNode/ParamTestNode.h>

struct StructForHash {
    ReteNodePtr Parent;
    AlphaMemoryPtr AlphaMemory;
    ParamTestNodeVector Tests;
    Condition Cond;

    bool operator== (const StructForHash& rhs) const;
};

namespace std {
    template<>
    struct std::hash<StructForHash> {
        std::size_t operator()(const StructForHash& obj) const {
            size_t ret = 2166136261;
            ret = (ret * 16777619) ^ hash<ReteNode>()(*obj.Parent);
            ret = (ret * 16777619) ^ hash<AlphaMemory>()(*obj.AlphaMemory);
            ret = (ret * 16777619) ^ hash<ParamTestNodeVector>()(obj.Tests);
            ret = (ret * 16777619) ^ hash<Condition>()(obj.Cond);
            return ret;
        }
    };
}