#pragma once

#include <Motor/Renderer/Prerequisites.h>

#include <Motor/Renderer/RHI/RenderSystem.h>

#include <Motor/Renderer/Resource/TextureManager.h>
#include <Motor/Renderer/Resource/MeshManager.h>
#include <Motor/Renderer/Resource/Material.h>

namespace motor {
class GraphicsEngine {
private:
    //Initialize the GraphicsEngine and DirectX 11 Device
    GraphicsEngine();
    //Release all the resources loaded
    ~GraphicsEngine();

public:
    RenderSystem* getRenderSystem();
    TextureManager* getTextureManager();
    MeshManager* getMeshManager();

public:
    MaterialPtr createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
    MaterialPtr createMaterial(const MaterialPtr& material);
    void setMaterial(const MaterialPtr& material);
    void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);
    void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials);

public:
    static GraphicsEngine* get();
    static void create();
    static void release();

private:
    RenderSystem* m_render_system = nullptr;
    TextureManager* m_tex_manager = nullptr;
    MeshManager* m_mesh_manager = nullptr;
    static GraphicsEngine* m_engine;

    unsigned char m_mesh_layout_byte_code[1024];
    size_t m_mesh_layout_size = 0;
};
} // namespace motor
