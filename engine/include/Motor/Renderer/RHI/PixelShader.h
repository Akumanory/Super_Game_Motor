#pragma once

#include <Motor/Renderer/Prerequisites.h>

#include <d3d11.h>

namespace motor {
class PixelShader {
public:
    PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
    ~PixelShader();

private:
    ID3D11PixelShader* m_ps = nullptr;
    RenderSystem* m_system = nullptr;

private:
    friend class RenderSystem;
    friend class DeviceContext;
};
} // namespace motor