#pragma once

#include "ReteNode.h"
#include "../AlphaPart/AlphaMemory.h"
#include "../TestNode/ParamTestNode.h"

class BetaNode : public ReteNode {
protected:

	ReteNodePtr LeftParent;
	AlphaMemoryPtr RightParent;
	ParamTestNodeVector Tests;

public:

	BetaNode(ReteNodePtr leftParent, AlphaMemoryPtr rightParent, const ParamTestNodeVector& tests);

	void ClearStatus() override;

};