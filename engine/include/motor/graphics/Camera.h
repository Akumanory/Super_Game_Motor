#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

private:
	void UpdateMatrix() override;

	XMMATRIX view_matrix;
	XMMATRIX projection_matrix;
};
