#pragma once

#include <motor/ECS/Components.h>
#include <entt/entt.hpp>

void SetPosition(PositionRotation& pos_struct, DirectX::XMFLOAT3 posVector);
void SetRotation(PositionRotation& rot_struct, DirectX::XMFLOAT3 rotVector);
void SetModel(ObjectModel& model_struct, const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
void UpdateBoundingBox(PositionRotation& pos_struct, ObjectModel& model_struct);