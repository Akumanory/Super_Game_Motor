#pragma once

#include <memory>

#include "../Structure/Node.h"
#include "../Structure/WME.h"

class AlphaMemory : public Node<WMEVector> {
public:

	void addWME(const WME& wme);

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