#pragma once

#include <motor/RBS/Beta/ReteNode.h>

struct DummyTopNode : public ReteNode {
public:

    DummyTopNode();

    void ClearStatus() override;

    const TokenVector& GetOutput() override;

};