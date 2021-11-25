#include <Motor/Renderer/Resource/TextureManager.h>
#include <Motor/Renderer/Resource/Texture.h>

using namespace motor;

TextureManager::TextureManager()
    : ResourceManager() {
}

TextureManager::~TextureManager() {
}

TexturePtr TextureManager::createTextureFromFile(const wchar_t* file_path) {
    return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}

TexturePtr TextureManager::createTexture(const Rect& size, Texture::Type type) {
    Texture* tex = nullptr;
    try {
        tex = new Texture(size, type);
    } catch (...) { }

    TexturePtr tex_ptr(tex);
    return tex_ptr;
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* file_path) {
    Texture* tex = nullptr;
    try {
        tex = new Texture(file_path);
    } catch (...) { }

    return tex;
}
