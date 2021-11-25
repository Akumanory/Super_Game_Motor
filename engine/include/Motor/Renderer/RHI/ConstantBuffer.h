#pragma once

#include <Motor/Renderer/Prerequisites.h>

namespace motor {
class ConstantBuffer {
public:
    ConstantBuffer(void* buffer, size_t size_buffer, RenderSystem* system);
    ~ConstantBuffer();

    void update(DeviceContextPtr context, void* buffer);

private:
    ID3D11Buffer* m_buffer = nullptr;
    RenderSystem* m_system = nullptr;

private:
    friend class DeviceContext;
};
} // namespace motor