#pragma once

#include <Motor/Renderer/Resource/ResourceManager.h>
#include <Motor/Renderer/Resource/Texture.h>

#include <SimpleMath.h>

namespace motor {

using namespace DirectX::SimpleMath;

class TextureManager : public ResourceManager {
public:
    TextureManager();
    ~TextureManager();

    TexturePtr createTextureFromFile(const wchar_t* file_path);
    TexturePtr createTexture(const Rectangle& size, Texture::Type type = Texture::Type::RenderTarget);

protected:
    virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};
} // namespace motor
