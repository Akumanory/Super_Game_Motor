#include <motor/RBS/ReteNet/Net.h>

#include <motor/RBS/Beta/JoinNode.h>
#include <motor/RBS/Beta/DummyTopNode.h>
#include <motor/RBS/Beta/TokenFilterNode.h>

#include "motor/ECS/Components.h"

ParamTestNodeVector Net::GetTestsFromCondition(Condition c, const ConditionVector & condsHigherUp) {
    ParamTestNodeVector ret;
    auto&& paramTests = ParamTestNode::Generate(c, condsHigherUp);
    std::copy(paramTests.begin(), paramTests.end(), std::back_inserter(ret));
    return ret;
}

ReteNodePtr Net::BuildOrShareJoinNode(ReteNodePtr parent, AlphaMemoryPtr am, 
    const ParamTestNodeVector & tests, const Condition& c) {
    StructForHash node = { parent, am, tests, c };
    auto&& it = dict.find(node);
    if (it == dict.end()) {
        auto&& ret = ReteNodePtr((ReteNode*)new JoinNode(parent, am, tests, c));
        dict.insert(std::make_pair(node, ret));
        return ret;
    }
    return it->second;
}

ReteNodePtr Net::BuildOrShareTokenFilterNode(ReteNodePtr parent, AlphaMemoryPtr am, 
    const ParamTestNodeVector & tests, const Condition& c) {
    StructForHash node = { parent, am, tests, c };
    auto&& it = dict.find(node);
    if (it == dict.end()) {
        auto&& ret = ReteNodePtr(new TokenFilterNode(parent, am, tests, c, testAtTokenFilterNode));
        dict.insert(std::make_pair(node, ret));
        return ret;
    }
    return it->second;
}

AlphaMemoryPtr Net::BuildOrShareAlphaMemory(const Condition & c) {
    auto&& index = c.GetIndex();
    if (conditionToAlphaMemory.count(index) == 0) {
        return conditionToAlphaMemory[index] = AlphaMemoryPtr(new AlphaMemory());
    }
    return conditionToAlphaMemory[index];
}

ReteNodePtr Net::BuildOrShareNetworkForConditions(ReteNodePtr parent, 
    const ConditionVector & conds, ConditionVector condsHigherUp) {
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

auto Net::from_json(rj::Value& obj) -> Net {
    Net net;
    auto& matches = obj["net"];

    for (auto& match : matches.GetArray()) {
        std::vector<Condition> productions;
        std::vector<Condition> functions;
        for (auto& cond : match[0].GetArray()) {
            Condition prod{ cond[0].GetString(), cond[1].GetString(), cond[2].GetString() };
            productions.push_back(prod);
        }
        for (auto& func : match[1].GetArray()) {
            Condition cond{ func[0].GetString(), func[1].GetString(), func[2].GetString() };
            functions.push_back(cond);
        }
    net.AddProduction({ productions }, { functions });
    }

    return net;
}
auto Net::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {

    rj::Value net_matches_j(rj::kArrayType);

    for (auto& i : this->resultNodes) {
        ConditionVector condProd = i.get()->getConds();
        rj::Value net_productions_j(rj::kArrayType);
        for (Condition con : condProd) {
            string id = con.get(Field::id);
            string attr = con.get(Field::attr);
            string value = con.get(Field::value);
            rj::Value id_j;
            id_j.SetString(id.c_str(), id.length(), rjAllocator);
            rj::Value attr_j;
            attr_j.SetString(attr.c_str(), attr.length(), rjAllocator);
            rj::Value value_j;
            value_j.SetString(value.c_str(), value.length(), rjAllocator);

            rj::Value prod_j(rj::kArrayType);
            prod_j.PushBack(id_j, rjAllocator);
            prod_j.PushBack(attr_j, rjAllocator);
            prod_j.PushBack(value_j, rjAllocator);

            net_productions_j.PushBack(prod_j, rjAllocator);
        }
        rj::Value net_functions_j(rj::kArrayType);
        ConditionVector condGetter = i.get()->getInfo();
        for (Condition con : condGetter) {
            string id = con.get(Field::id);
            string attr = con.get(Field::attr);
            string value = con.get(Field::value);
            rj::Value id_j;
            id_j.SetString(id.c_str(), id.length(), rjAllocator);
            rj::Value attr_j;
            attr_j.SetString(attr.c_str(), attr.length(), rjAllocator);
            rj::Value value_j;
            value_j.SetString(value.c_str(), value.length(), rjAllocator);
            rj::Value function_j(rj::kArrayType);
            function_j.PushBack(id_j, rjAllocator);
            function_j.PushBack(attr_j, rjAllocator);
            function_j.PushBack(value_j, rjAllocator);

            net_functions_j.PushBack(function_j, rjAllocator);
        }
        rj::Value net_match_j(rj::kArrayType);

        net_match_j.PushBack(net_productions_j, rjAllocator);
        net_match_j.PushBack(net_functions_j, rjAllocator);

        net_matches_j.PushBack(net_match_j, rjAllocator);
    }
    obj.AddMember("net", net_matches_j, rjAllocator);
}

Net::Net() : RootNode(ReteNodePtr((ReteNode*)(new DummyTopNode()))) {}

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

            Condition me = FindProduction(id, attr);
            auto&& it = conditionToAlphaMemory.find(me);

            for (auto&& value : vt.at(2)) {

                it->second->removeWME(id, attr);

                string match_string = it->first.get(Field::value);
                if (match_string.at(0) == '>') {
                    match_string.erase(match_string.find('>'), 1);
                    float wme_value;
                    float match_value;
                    std::istringstream(value) >> wme_value;
                    std::istringstream(match_string) >> match_value;
                    if (wme_value > match_value)
                        it->second->addWME(wme);
                    return;
                }

                if (match_string.at(0) == '<') {
                    match_string.erase(match_string.find('<'), 1);
                    float wme_value;
                    float match_value;
                    std::istringstream(value) >> wme_value;
                    std::istringstream(match_string) >> match_value;
                    if (wme_value < match_value)
                        it->second->addWME(wme);
                    return;
                }

                if (match_string.at(0) == '!') {
                    match_string.erase(match_string.find('!'), 1);
                    if (value != match_string)
                        it->second->addWME(wme);
                    return;
                }

                if (value == match_string)
                    it->second->addWME(wme);
            }
        }
    }
}

Condition Net::FindProduction(string id, string attr) {
    std::vector<std::pair<const Condition, AlphaMemoryPtr>> v(conditionToAlphaMemory.begin(), conditionToAlphaMemory.end());
    for (std::pair<Condition, AlphaMemoryPtr> cond : v) {
        i = i + 1;
        if ((cond.first.get(Field::id) == id) || (cond.first.get(Field::attr) == attr))
            return cond.first;
    }
    Condition cond = { "", "", "" };
    return cond;
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