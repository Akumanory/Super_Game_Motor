#pragma once

//#include <unordered_map>
//#include <unordered_set>

#include "../TestNode/TestAtTokenFilterNode.h"
#include "../Structure/Condition.h"
#include "../Alpha/AlphaMemory.h"
#include "../Beta/ProductionNode.h"
#include "../TestNode/ParamTestNode.h"

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

class Net {

	ReteNodePtr RootNode;
	TestAtTokenFilterNode testAtTokenFilterNode;
	std::unordered_map<Condition, AlphaMemoryPtr> conditionToAlphaMemory;
	std::unordered_map<StructForHash, ReteNodePtr> dict;
	std::unordered_set<ProductionNodePtr> resultNodes;

	ParamTestNodeVector GetTestsFromCondition(Condition c, 
		const ConditionVector& condsHigherUp);

	ReteNodePtr BuildOrShareJoinNode(ReteNodePtr parent, AlphaMemoryPtr am, 
		const ParamTestNodeVector& tests, const Condition& c);

	ReteNodePtr BuildOrShareTokenFilterNode(ReteNodePtr parent, AlphaMemoryPtr am, 
		const ParamTestNodeVector & tests, const Condition& c);

	AlphaMemoryPtr BuildOrShareAlphaMemory(const Condition& c);

	ReteNodePtr BuildOrShareNetworkForConditions(ReteNodePtr parent, 
		const ConditionVector& conds, ConditionVector condsHigherUp);

public:

	Net();

	void AddProduction(const ConditionVector& conditions, const std::vector<Condition>& getter);

	std::vector<ConditionVector> invoke();

	void ClearStatus();

	void AddWME(const WME& wme);

	void AddFunction(const std::string& key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction);

};