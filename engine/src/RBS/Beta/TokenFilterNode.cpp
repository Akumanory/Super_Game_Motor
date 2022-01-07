#include <motor/RBS/Beta/TokenFilterNode.h>

#include <motor/RBS/TestNode/TestAtTokenFilterNode.h>
#include <motor/RBS/PublicDefine.h>

TokenFilterNode::TokenFilterNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent,
    const ParamTestNodeVector & tests, const Condition& c, TestAtTokenFilterNode& testAtTokenFilterNode)
    : BetaNode(leftParent, rightParent, tests), c(c), testAtTokenFilterNode(testAtTokenFilterNode) {
    switch (c.GetType())
    {
    case Condition::positive:
        isPositive = true;
        break;
    case Condition::negative:
        isPositive = false;
        break;
    default:
        myAssert(false)
    }
}

const TokenVector & TokenFilterNode::GetOutput() {
    if (!isReadyForOutput) {
        // Can be optimized
        for (auto&& token : LeftParent->GetOutput()) {
            for (auto&& test : Tests) {
                c.set(
                    test.FieldName, 
                    token.at(test.ConditionNumberOfArg).get(test.FieldOfArg)
                );
            }
            if ((isPositive ^ testAtTokenFilterNode.PerformTest(c)) == 0) {
                output.push_back(token);
                output.back().push_back({ 
                    c.get(Field::id), 
                    (isPositive ? "" : "~") + c.get(Field::attr),
                    c.get(Field::value)
                });
            }
        }
        isReadyForOutput = true;
    }
    return output;
}
