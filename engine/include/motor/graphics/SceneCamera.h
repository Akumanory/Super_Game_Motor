#pragma once 

#include <DirectXMath.h>
#include <DirectXCollision.h>

class SceneCamera
{
public:

    // TODO: Добавить поддержку фрустум куллинга

    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetViewPortSize(int height, int width);
    void SetProjectionValues(float fovDegrees, float nearZ, float farZ);
    
    void SetVerticalFOV(float FOV);
    float GetVerticalFOV();

    void SetNearZ(float nearZ);
    float GetNearZ();

    void SetFarZ(float farZ);
    float GetFarZ();

    DirectX::XMMATRIX& GetProjection();
    DirectX::BoundingFrustum& GetFrustum();
    void UpdateProjection();

private:
    float fov_degrees = 90.0f;
    float aspect_ratio = 1.0f;
    float near_z = 0.1f;
    float far_z = 1000.0f;

    DirectX::BoundingFrustum base_frustum;

    DirectX::XMMATRIX camera_projection;
};