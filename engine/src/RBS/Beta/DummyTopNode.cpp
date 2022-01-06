#include "DummyTopNode.h"

DummyTopNode::DummyTopNode()
	: ReteNode(true) {
	output.push_back({});
}

void DummyTopNode::ClearStatus() {
}

const TokenVector & DummyTopNode::GetOutput() {
	return output;
}
