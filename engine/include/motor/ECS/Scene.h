#pragma once

#include <motor/other/ModelLoader.h>
#include <entt/entt.hpp>


//struct RenderableEntities
//{
//    TransformComponent& transform_comp;
//    MeshComponent& mesh_comp;
//
//    //RenderableEntities() = default;
//};

class Entity;

class Scene
{
public:
    Scene();
    Scene(int& height, int& width, ModelLoader* model_manager);
    ~Scene();

    void SetModelLoader(ModelLoader* model_manager);
    void SetAspectRatioParams(int height, int width);
    Entity CreateEntity(const std::string name);
    void DestroyEntity(Entity entity);
    std::vector<Entity> GetRenderableEntities();
    std::vector<Entity> GetCamerasEntities();
    Entity GetPrimaryCamera();
    
    void OnRednerUpdate();

    void RenderDialogs();

    /*void Initialize(ModelLoader& model_loader);
    void InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    void DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix, DirectX::BoundingFrustum& f_culling);
    void AddSimpleCube(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader, DirectX::XMFLOAT3 pos);*/

    void Save();
    void Load();

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

private:
    int m_window_height;
    int m_window_width;

    entt::registry m_registry;
    ModelLoader* m_model_manager;

    friend class Entity;
    friend class SceneHierarchy;
};