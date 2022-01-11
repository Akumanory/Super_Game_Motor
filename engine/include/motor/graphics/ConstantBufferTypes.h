#pragma once

#include <DirectXMath.h>
#include <motor/graphics/PointLightStruct.h>

struct CB_VS_vertex_shader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct CB_PS_light_shader
{
	DirectX::XMFLOAT3 ambientLightColor; // 12 byte
	float ambientLightStrength; // 4 bytes => alight for 16 bytes
	// 16

	PointLightStruct lights[100];

	int size; // 4 bytes;
};