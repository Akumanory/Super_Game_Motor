#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../Structure/Condition.h"
#include "../Structure/Token.h"
#include "../Structure/WME.h"

struct ParamTestNode {
	using ParamTestNodeVector = std::vector<ParamTestNode>;

	Field::Name FieldName;
	size_t ConditionNumberOfArg;
	Field::Name FieldOfArg;

	ParamTestNode(Field::Name fieldName, size_t conditionNumberOfArg
		, Field::Name fieldOfArg);
	bool operator== (const ParamTestNode& rhs) const;
	bool PerformTest(const Token& token, const WME& wme) const;
	static ParamTestNodeVector Generate(const Condition& c, const ConditionVector& condsHigherUp);
};

using ParamTestNodeVector = std::vector<ParamTestNode>;

namespace std {
	template<>
	struct hash<ParamTestNode> {
		size_t operator() (const ParamTestNode& node) const {
			size_t ret = 2166136261;
			ret = (ret * 16777619) ^ hash<int>()(node.FieldName);
			ret = (ret * 16777619) ^ hash<size_t>()(node.ConditionNumberOfArg);
			ret = (ret * 16777619) ^ hash<int>()(node.FieldOfArg);
			return ret;
		}
	};
	template<>
	struct hash<ParamTestNodeVector> {
		size_t operator() (const ParamTestNodeVector& nodes) const {
			size_t ret = 2166136261;
			for (auto&& node : nodes)
				ret = (ret * 16777619) ^ hash<ParamTestNode>()(node);
			return ret;
		}
	};
}