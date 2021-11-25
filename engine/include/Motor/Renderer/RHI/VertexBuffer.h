#pragma once

#include <Motor/Renderer/Prerequisites.h>

namespace motor {
class VertexBuffer {
public:
    VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader,
      RenderSystem* system);
    UINT getSizeVertexList();
    ~VertexBuffer();

private:
    UINT m_size_vertex;
    UINT m_size_list;

private:
    ID3D11Buffer* m_buffer = nullptr;
    ID3D11InputLayout* m_layout = nullptr;
    RenderSystem* m_system = nullptr;

private:
    friend class DeviceContext;
};
} // namespace motor