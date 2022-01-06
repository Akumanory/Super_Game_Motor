#pragma once

#include <iterator>

#include "BetaNode.h"
#include "../Structure/Condition.h"

class JoinNode : public BetaNode {
public:

	JoinNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent, 
		const ParamTestNodeVector& tests, const Condition& c);

	const TokenVector& GetOutput() override;

private:

	bool isPositive;

};