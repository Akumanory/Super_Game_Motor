#pragma once

#include <Motor/Renderer/Resource/Resource.h>

#include <d3d11.h>
#include <SimpleMath.h>

namespace motor {

using namespace DirectX::SimpleMath;

class Texture : public Resource {
public:
    enum Type {
        Normal = 0,
        RenderTarget,
        DepthStencil
    };

public:
    Texture(const wchar_t* full_path);
    Texture(const Rectangle& size, Texture::Type type = RenderTarget);
    Rectangle getSize();
    ~Texture();

private:
    ID3D11Resource* m_texture = nullptr;
    ID3D11ShaderResourceView* m_shader_res_view = nullptr;
    ID3D11SamplerState* m_sampler_state = nullptr;
    ID3D11RenderTargetView* m_render_target_view = nullptr;
    ID3D11DepthStencilView* m_depth_stencil_view = nullptr;

    Texture::Type m_type = Texture::Type::Normal;

    Rectangle m_size;

private:
    friend class DeviceContext;
};
} // namespace motor
