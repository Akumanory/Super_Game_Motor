#pragma once

#include <motor/RBS/Beta/ReteNode.h>
#include <motor/RBS/TestNode/ParamTestNode.h>
#include <motor/RBS/Alpha/AlphaMemory.h>

class BetaNode : public ReteNode {
protected:

    ReteNodePtr LeftParent;
    AlphaMemoryPtr RightParent;
    ParamTestNodeVector Tests;

public:

    BetaNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent, const ParamTestNodeVector& tests);

    void ClearStatus() override;

};
