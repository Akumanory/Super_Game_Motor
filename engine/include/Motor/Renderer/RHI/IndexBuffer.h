#pragma once

#include <Motor/Renderer/Prerequisites.h>

#include <d3d11.h>

namespace motor {
class IndexBuffer {
public:
    IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
    UINT getSizeIndexList();
    ~IndexBuffer();

private:
    UINT m_size_list;

private:
    ID3D11Buffer* m_buffer = nullptr;
    RenderSystem* m_system = nullptr;

private:
    friend class DeviceContext;
};
} // namespace motor