#include "Net.h"

#include "../BetaPart/JoinNode.h"
#include "../BetaPart/DummyTopNode.h"
#include "../BetaPart/TokenFilterNode.h"

ParamTestNodeVector Net::GetTestsFromCondition(Condition c, const ConditionVector & condsHigherUp) {
	ParamTestNodeVector ret;
	auto&& paramTests = ParamTestNode::Generate(c, condsHigherUp);
	std::copy(paramTests.begin(), paramTests.end(), std::back_inserter(ret));
	return ret;
}

ReteNodePtr Net::BuildOrShareJoinNode(ReteNodePtr parent, AlphaMemoryPtr am
	, const ParamTestNodeVector & tests, const Condition& c) {
	StructForHash node = { parent, am, tests, c };
	auto&& it = dict.find(node);
	if (it == dict.end()) {
		auto&& ret = ReteNodePtr((ReteNode*)new JoinNode(parent, am, tests, c));
		dict.insert(std::make_pair(node, ret));
		return ret;
	}
	else
		return it->second;
}

ReteNodePtr Net::BuildOrShareTokenFilterNode(ReteNodePtr parent, AlphaMemoryPtr am
	, const ParamTestNodeVector & tests, const Condition& c) {
	StructForHash node = { parent, am, tests, c };
	auto&& it = dict.find(node);
	if (it == dict.end()) {
		auto&& ret = ReteNodePtr(new TokenFilterNode(parent, am, tests, c, testAtTokenFilterNode));
		dict.insert(std::make_pair(node, ret));
		return ret;
	}
	else
		return it->second;
}

AlphaMemoryPtr Net::BuildOrShareAlphaMemory(const Condition & c) {
	auto&& index = c.GetIndex();
	if (conditionToAlphaMemory.count(index) == 0) {
		return conditionToAlphaMemory[index] = AlphaMemoryPtr(new AlphaMemory());
	}
	return conditionToAlphaMemory[index];
}

ReteNodePtr Net::BuildOrShareNetworkForConditions(ReteNodePtr parent
	, const ConditionVector & conds, ConditionVector condsHigherUp) {
	for (auto&& c : conds) {
		auto&& tests = GetTestsFromCondition(c, condsHigherUp);
		auto&& am = BuildOrShareAlphaMemory(c);
		if (testAtTokenFilterNode.IsNeedFilterToken(c)) {
			parent = BuildOrShareTokenFilterNode(parent, am, tests, c);
		}
		else {
			parent = BuildOrShareJoinNode(parent, am, tests, c);
		}
		condsHigherUp.push_back(c);
	}
	return parent;
}

Net::Net() : RootNode(ReteNodePtr((ReteNode*)(new DummyTopNode()))) {
}

size_t i = 0;
void Net::AddProduction(const ConditionVector & conditions, const std::vector<Condition>& getter) {
	auto&& curentNode = BuildOrShareNetworkForConditions(RootNode, conditions, {});
	resultNodes.insert(ProductionNodePtr(new ProductionNode(curentNode, conditions, getter)));
}

std::vector<ConditionVector> Net::invoke() {
	std::vector<ConditionVector> ret;
	for (auto&& node : resultNodes) {
		auto&& infos = node->getOutputInfos();
		std::copy(infos.begin(), infos.end(), std::back_inserter(ret));
	}
	return ret;
}

void Net::ClearStatus() {
	for (auto&& node : resultNodes) {
		node->ClearStatus();
	}
}

void Net::AddWME(const WME & wme) {
	std::vector<std::vector<std::string>> vt = {
		{ wme.get(Field::id), Condition::ArbitraryString },
		{ wme.get(Field::attr), Condition::ArbitraryString },
		{ wme.get(Field::value), Condition::ArbitraryString },
	};
	for (auto&& id : vt.at(0)) {
		for (auto&& attr : vt.at(1)) {
			for (auto&& value : vt.at(2)) {
				auto&& it = conditionToAlphaMemory.find(Condition(id, attr, value));
				if (it != conditionToAlphaMemory.end())
					it->second->addWME(wme);
			}
		}
	}
}

void Net::AddFunction(const std::string & key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction) {
	testAtTokenFilterNode.InsertJudgeFunction(key, judgeFunction);
}

bool StructForHash::operator==(const StructForHash & rhs) const {
	return *Parent == *rhs.Parent && 
		Tests == rhs.Tests && 
		*AlphaMemory == *rhs.AlphaMemory && 
		Cond == rhs.Cond;
}