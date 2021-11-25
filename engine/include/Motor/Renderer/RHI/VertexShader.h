#pragma once

#include <Motor/Renderer/Prerequisites.h>

#include <d3d11.h>

namespace motor {
class VertexShader : public Object {
public:
    VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
    ~VertexShader();

private:
    ID3D11VertexShader* m_vs = nullptr;
    RenderSystem* m_system = nullptr;

private:
    friend class RenderSystem;
    friend class DeviceContext;
};
} // namespace motor
