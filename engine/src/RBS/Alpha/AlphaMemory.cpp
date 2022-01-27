#include <motor/RBS/Alpha/AlphaMemory.h>

void AlphaMemory::addWME(const WME & wme) {
    output.push_back(wme);
}

void AlphaMemory::removeWME(string id, string attr) {
    int iterator = -1;
    for (auto v : output) {
        iterator = iterator + 1;
        if ((v.get(Field::id) == id) || (v.get(Field::attr) == attr)) {
            cout << attr;
            output.erase(output.begin() + iterator);
            return;
        }
    }
}

const WMEVector & AlphaMemory::GetOutput() {
    return output;
}

void AlphaMemory::ClearStatus() {
    output.clear();
}
