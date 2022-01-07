#include <motor/RBS/TestNode/ParamTestNode.h>

#include <motor/RBS/PublicDefine.h>

ParamTestNode::ParamTestNode(Field::Name fieldName, 
    size_t conditionNumberOfArg, Field::Name fieldOfArg)
    : FieldName(fieldName), ConditionNumberOfArg(conditionNumberOfArg),
    FieldOfArg(fieldOfArg) {
}

bool ParamTestNode::operator==(const ParamTestNode & rhs) const {
    return FieldName == rhs.FieldName
        && ConditionNumberOfArg == rhs.ConditionNumberOfArg
        && FieldOfArg == rhs.FieldOfArg;
}

bool ParamTestNode::PerformTest(const Token & token, const WME & wme) const {
    return token.at(ConditionNumberOfArg).get(FieldOfArg) == wme.get(FieldName);
}

ParamTestNodeVector ParamTestNode::Generate(const Condition& c, const ConditionVector& condsHigherUp) {
    ParamTestNodeVector result;
    std::unordered_map<std::string, std::pair<size_t, Field::Name>> dict;
    for (size_t i = 0; i < condsHigherUp.size(); ++i) {
        auto&& cond = condsHigherUp.at(i);
        if (cond.GetType() == Condition::positive) {
            for (auto&& fieldName : { Field::id, Field::attr, Field::value }) {
                auto&& fieldString = cond.get(fieldName);
                if (Field::isParam(fieldString)) {
                        auto&& paramName = Field::getParamName(fieldString);
                        dict.insert_or_assign(paramName, std::make_pair(i, fieldName));
                }
            }
        }
    }
    std::unordered_set<std::string> checkSet;
    for (auto&& fieldName : { Field::id, Field::attr, Field::value }) {
        auto&& fieldString = c.get(fieldName);
        if (Field::isParam(fieldString)) {
            auto&& paramName = Field::getParamName(fieldString);
            auto&& it = dict.find(paramName);
            if (it != dict.end()) {
                result.emplace_back(fieldName, it->second.first, it->second.second);
            }
            else {
                myAssert(checkSet.count(paramName) == 0);
                checkSet.insert(paramName);
            }
        }
    }
    return result;
}
