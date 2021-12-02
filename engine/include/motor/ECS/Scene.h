#pragma once
#include <motor/ECS/Components.h>
#include <motor/ECS/Factory.h>
#include <motor/ECS/CompRealization.h>
#include <entt/entt.hpp>
#include <motor/ECS/ComponentSystems.h>
#include <motor/other/ModelLoader.h>

struct RenderableEntities
{
    TransformComponent& transform_comp;
    MeshComponent& mesh_comp;

    //RenderableEntities() = default;
};


class Scene
{
public:
    void Initialize(ModelLoader& model_loader);
    void InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    void DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix, DirectX::BoundingFrustum& f_culling);
    void AddSimpleCube(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader, DirectX::XMFLOAT3 pos);

    std::vector<RenderableEntities> GetRenderableEntities();
    

private:
    //void DrawMesh(MeshComponent& mesh_comp, TransformComponent& transform_comp, const XMMATRIX& viewProjectionMatrix);

    ModelLoader _model_loader;

    std::vector<entt::entity> entities;
    entt::registry m_registry;
};