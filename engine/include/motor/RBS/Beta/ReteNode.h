#pragma once

#include <memory>

#include <motor/RBS/Structure/Node.h>
#include <motor/RBS/Structure/Token.h>

class ReteNode : public Node<TokenVector> {
protected:

    bool isReadyForOutput;

public:

    ReteNode(bool isReadyForOutput);

};

using ReteNodePtr = std::shared_ptr<ReteNode>;

namespace std {
    template<>
    struct std::hash<ReteNode> {
        size_t operator()(const ReteNode& node) const {
            return hash<size_t>()(node.serialNumber);
        }
    };
}