#include <Motor/Renderer/RHI/GraphicsEngine.h>
#include <Motor/Renderer/RHI/RenderSystem.h>
#include <Motor/Renderer/RHI/DeviceContext.h>

#include <exception>

using namespace motor;

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine() {
    try {
        m_render_system = new RenderSystem();
    } catch (...) { throw std::exception("RenderSystem not created successfully"); }

    try {
        m_tex_manager = new TextureManager();
    } catch (...) { throw std::exception("TextureManager not created successfully"); }

    try {
        m_mesh_manager = new MeshManager();
    } catch (...) { throw std::exception("MeshManager not created successfully"); }

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;
    m_render_system->compileVertexShader(L"shaders/VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
    ::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
    m_mesh_layout_size = size_shader;
    m_render_system->releaseCompiledShader();
}

RenderSystem* GraphicsEngine::getRenderSystem() {
    return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager() {
    return m_tex_manager;
}

MeshManager* GraphicsEngine::getMeshManager() {
    return m_mesh_manager;
}

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path) {
    MaterialPtr mat = nullptr;
    try {
        mat = std::make_shared<Material>(vertex_shader_path, pixel_shader_path);
    } catch (...) { }

    return mat;
}

MaterialPtr GraphicsEngine::createMaterial(const MaterialPtr& material) {
    MaterialPtr mat = nullptr;
    try {
        mat = std::make_shared<Material>(material);
    } catch (...) { }

    return mat;
}

void GraphicsEngine::setMaterial(const MaterialPtr& material) {
    GraphicsEngine::get()->getRenderSystem()->setRasterizerState((material->m_cull_mode == CULL_MODE_FRONT));

    if (material->m_constant_buffer) {
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_vertex_shader, material->m_constant_buffer);
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_pixel_shader, material->m_constant_buffer);
    }
    //SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->m_vertex_shader);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->m_pixel_shader);

    if (material->m_vec_textures.size()) {
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(material->m_pixel_shader, &material->m_vec_textures[0], (UINT)material->m_vec_textures.size());
    }
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size) {
    *byte_code = m_mesh_layout_byte_code;
    *size = m_mesh_layout_size;
}

void GraphicsEngine::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials) {
    for (unsigned int m = 0; m < list_materials.size(); m++) {
        if (m == list_materials.size())
            break;

        MaterialSlot mat = mesh->getMaterialSlot(m);

        GraphicsEngine::get()->setMaterial(list_materials[m]);
        //SET THE VERTICES OF THE TRIANGLE TO DRAW
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
        //SET THE INDICES OF THE TRIANGLE TO DRAW
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
        // FINALLY DRAW THE TRIANGLE
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList((UINT)mat.num_indices, 0, (UINT)mat.start_index);
    }
}

GraphicsEngine::~GraphicsEngine() {
    GraphicsEngine::m_engine = nullptr;
    delete m_mesh_manager;
    delete m_tex_manager;
    delete m_render_system;
}

void GraphicsEngine::create() {
    if (GraphicsEngine::m_engine) {
        throw std::exception("Graphics Engine already created");
    }

    GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release() {
    if (!GraphicsEngine::m_engine)
        return;
    delete GraphicsEngine::m_engine;
}

GraphicsEngine* GraphicsEngine::get() {
    return m_engine;
}
