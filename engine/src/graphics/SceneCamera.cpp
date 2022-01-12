#include <motor/graphics/SceneCamera.h>

SceneCamera::SceneCamera() 
{
    UpdateProjection();
}

void SceneCamera::SetViewPortSize(int height, int width) 
{
    aspect_ratio = float(width) / float(height);
    UpdateProjection();
}

void SceneCamera::SetProjectionValues(float fovDegrees, float nearZ, float farZ) 
{
    fov_degrees = fovDegrees;
    near_z = nearZ;
    far_z = farZ;
    UpdateProjection();
}

void SceneCamera::SetVerticalFOV(float FOV) 
{
    fov_degrees = FOV;
    UpdateProjection();
}

float SceneCamera::GetVerticalFOV()
{
    return fov_degrees;
}

void SceneCamera::SetNearZ(float nearZ) 
{
    near_z = nearZ;
    UpdateProjection();
}

float SceneCamera::GetNearZ() 
{
    return near_z;
}

void SceneCamera::SetFarZ(float farZ) 
{
    far_z = farZ;
    UpdateProjection();
}

float SceneCamera::GetFarZ() 
{
    return far_z;
}

DirectX::XMMATRIX& SceneCamera::GetProjection() 
{
    return camera_projection;
}

DirectX::BoundingFrustum& SceneCamera::GetFrustum() 
{
    return base_frustum;
}

void SceneCamera::UpdateProjection() 
{
    float fovRadians = (fov_degrees / 360.0f) * DirectX::XM_2PI;
    camera_projection = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspect_ratio, near_z, far_z);
    DirectX::BoundingFrustum::CreateFromMatrix(base_frustum, camera_projection);
}