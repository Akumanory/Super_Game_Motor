#include <Renderer/RHI/VertexShader.h>
#include <Renderer/RHI/RenderSystem.h>

#include <exception>

using namespace motor;

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system)
    : m_system(system) {
    if (FAILED(m_system->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs))) {
        throw std::exception("VertexShader not created successfully")
    }
}

VertexShader::~VertexShader() {
    m_vs->Release();
}
