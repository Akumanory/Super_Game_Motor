#pragma once

#include <DirectXMath.h>

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

	DirectX::XMFLOAT3 dynamicLightColor; // 12 byte
	float dynamicLightStrength; // 4 bytes => alight for 16 bytes
	//16

	DirectX::XMFLOAT3 dynamicLightPosition; // 12 byte
	float dynamicLightAttenuation_A; // 4 bytes
	// 16

	float dynamicLightAttenuation_B; // 4 bytes
	float dynamicLightAttenuation_C; // 4 bytes
};