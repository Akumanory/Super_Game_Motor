#include <motor/RBS/Beta/BetaNode.h>

#include <motor/RBS/Alpha/AlphaMemory.h>

BetaNode::BetaNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent, const ParamTestNodeVector& tests)
    : ReteNode(false), LeftParent(leftParent), RightParent(rightParent), Tests(tests) {
}

void BetaNode::ClearStatus() {
    if (isReadyForOutput) {
        isReadyForOutput = false;
        output.clear();
        LeftParent->ClearStatus();
        RightParent->ClearStatus();
    }
}
