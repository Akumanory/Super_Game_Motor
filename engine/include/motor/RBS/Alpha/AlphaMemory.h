#pragma once

#include <memory>
#include <motor/RBS/Structure/Node.h>
#include <motor/RBS/Structure/WME.h>

class AlphaMemory : public Node<WMEVector> {
public:

    void addWME(const WME& wme);
    void removeWME(string id, string attr);

    const WMEVector& GetOutput() override;

    void ClearStatus() override;

};

using AlphaMemoryPtr = std::shared_ptr<AlphaMemory>;

namespace std {
    template<>
    struct hash<AlphaMemory> {
        size_t operator()(const AlphaMemory& am) const {
            return hash<size_t>()(am.serialNumber);
        }
    };
}