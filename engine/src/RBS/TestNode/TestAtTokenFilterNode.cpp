#include <motor/RBS/TestNode/TestAtTokenFilterNode.h>

#include <motor/RBS/StringOp.h>
#include <motor/RBS/PublicDefine.h>

bool TestAtTokenFilterNode::PerformTest(const Condition & c)
{
    using namespace Field;
    auto&& it = dict.find(c.get(attr));
    myAssert(it != dict.end());
    return it->second(c.get(id), c.get(value));
}

bool TestAtTokenFilterNode::IsNeedFilterToken(const Condition & c)
{
    auto&& value = c.get(Field::value);
    if (value.empty()) {
        myAssertPlus(dict.count(value) > 0, value);
        return true;
    }
    return false;
}

void TestAtTokenFilterNode::InsertJudgeFunction(const std::string & key, JudgeFunctionType & value) {
    auto&& result = dict.insert(std::make_pair(key, value));
    myAssertPlus(result.second, key);
}
