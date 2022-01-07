#include <motor/RBS/Beta/JoinNode.h>

#include <motor/RBS/PublicDefine.h>

JoinNode::JoinNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent, 
    const ParamTestNodeVector& tests, const Condition& c)
    : BetaNode(leftParent, rightParent, tests) {
    switch (c.GetType())
    {
    case Condition::positive:
        isPositive = true;
        break;
    case Condition::negative:
        isPositive = false;
        break;
    default:
        myAssert(false);
    }
}

const TokenVector & JoinNode::GetOutput() {
    if (!isReadyForOutput) {
        // Can Be Optimized
        for (auto&& token : LeftParent->GetOutput()) {
            if (isPositive) {
                for (auto&& wme : RightParent->GetOutput()) {
                    bool passAllCheck = true;
                    for (auto&& test : Tests) {
                        if (!test.PerformTest(token, wme)) {
                            passAllCheck = false;
                            break;
                        }
                    }
                    if (passAllCheck) {
                        output.push_back(token);
                        output.back().push_back(wme);
                    }
                }
        }
        else {
                bool passNoneCheck = true;
                for (auto&& wme : RightParent->GetOutput()) {
                    for (auto&& test : Tests) {
                        if (test.PerformTest(token, wme)) {
                            passNoneCheck = false;
                            break;
                        }
                    }
                    if (!passNoneCheck)
                        break;
                }
                if (passNoneCheck) {
                    output.push_back(token);
                    output.back().push_back({ "?", "?", "?" });
                }
            }
        }
        isReadyForOutput = true;
    }
    return output;
}