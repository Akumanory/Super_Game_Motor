#pragma once
#include <motor/ECS/Components.h>
#include <motor/ECS/Factory.h>
#include <motor/ECS/CompRealization.h>
#include <entt/entt.hpp>

class Scene
{
public:
    void InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    void DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix);

private:
    entt::entity first_entity;
    entt::registry m_registry;
};