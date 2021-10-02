#pragma once

#include <motor/graphics/RenderableGameObject.h>

class Light : public RenderableGameObject
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);

	DirectX::XMFLOAT3 lightColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float lightStrength = 1.0f;

	float attennuation_A = 1.0f;
	float attennuation_B = 0.1f;
	float attennuation_C = 0.1f;
};