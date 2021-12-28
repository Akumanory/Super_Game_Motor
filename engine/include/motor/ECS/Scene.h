#pragma once
#include <motor/ECS/Components.h>
#include <motor/ECS/Factory.h>
#include <entt/entt.hpp>


struct RenderableEntities
{
    TransformComponent& transform_comp;
    MeshComponent& mesh_comp;

    //RenderableEntities() = default;
};

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string name);
    void DestroyEntity(Entity entity);
    std::vector<Entity> GetRenderableEntities();

    /*void Initialize(ModelLoader& model_loader);
    void InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    void DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix, DirectX::BoundingFrustum& f_culling);
    void AddSimpleCube(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader, DirectX::XMFLOAT3 pos);*/

    void Save();
    void Load();

private:
    entt::registry m_registry;
    ModelLoader m_model_manager;

    friend class Entity;
    friend class SceneHierarchy;
};