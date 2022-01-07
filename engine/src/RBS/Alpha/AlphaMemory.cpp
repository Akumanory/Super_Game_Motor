#include <motor/RBS/Alpha/AlphaMemory.h>

void AlphaMemory::addWME(const WME & wme) {
    output.push_back(wme);
}

const WMEVector & AlphaMemory::GetOutput() {
    return output;
}

void AlphaMemory::ClearStatus() {
    output.clear();
}
