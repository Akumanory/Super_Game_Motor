#pragma once

#include <motor/RBS/Beta/BetaNode.h>
#include <motor/RBS/TestNode/TestAtTokenFilterNode.h>

class TokenFilterNode : public BetaNode {
public:

    TokenFilterNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent,
        const ParamTestNodeVector & tests, const Condition& c, TestAtTokenFilterNode& testAtTokenFilterNode);

    const TokenVector& GetOutput() override;

private:

    Condition c;
    bool isPositive;
    TestAtTokenFilterNode& testAtTokenFilterNode;

};