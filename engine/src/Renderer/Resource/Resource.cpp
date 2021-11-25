#include <Motor/Renderer/Resource/Resource.h>

using namespace motor;

Resource::Resource(const wchar_t* full_path)
    : m_full_path(full_path) {
}

Resource::~Resource() {
}
