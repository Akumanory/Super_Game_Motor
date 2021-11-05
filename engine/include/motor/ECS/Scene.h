#pragma once
#include <motor/ECS/Components.h>
#include <motor/ECS/Factory.h>
#include <motor/ECS/CompRealization.h>
#include <entt/entt.hpp>

class Scene
{
public:
    void InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    void DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix, DirectX::BoundingFrustum& f_culling);
    void AddSimpleCube(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader, DirectX::XMFLOAT3 pos);

private:
    entt::registry m_registry;
};