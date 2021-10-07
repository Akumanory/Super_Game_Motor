#pragma once

#include <DirectXMath.h>


// TODO придумать адекватную архитектуру а то сейчас через пень колоду
class FrustumCulling 
{
public:
    void CreateFromCameraProjection(DirectX::XMMATRIX camera_projection_matrix);
    void Transform(FrustumCulling& non_local_f_culling, DirectX::XMMATRIX view_to_local);
    bool ContainsByPositonPoint(DirectX::XMVECTOR position);
    

    DirectX::XMFLOAT3 _origin;
    DirectX::XMFLOAT4 _orientation;
private:
    DirectX::XMVECTOR PlaneTransform(DirectX::XMVECTOR plane, DirectX::XMVECTOR rotation, DirectX::XMVECTOR translation);
    bool ContainedBy(DirectX::FXMVECTOR Plane0, DirectX::FXMVECTOR Plane1, DirectX::FXMVECTOR Plane2,
      DirectX::GXMVECTOR Plane3, DirectX::HXMVECTOR Plane4, DirectX::HXMVECTOR Plane5, DirectX::XMVECTOR position);
    void IntersectWithPlane(DirectX::FXMVECTOR Center, DirectX::FXMVECTOR Extents, DirectX::FXMVECTOR Plane,
      DirectX::XMVECTOR& Outside, DirectX::XMVECTOR& Inside);

    float _rightSlope;
    float _leftSlope;
    float _topSlope;
    float _bottomSlope;

    float _nearZ;
    float _farZ;
};