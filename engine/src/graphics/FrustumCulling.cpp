#include <motor/graphics/FrustumCulling.h>

void FrustumCulling::CreateFromCameraProjection(DirectX::XMMATRIX camera_projection_matrix) 
{
    static DirectX::XMVECTORF32 HomogenousPoints[6] = {
        { { { 1.0f, 0.0f, 1.0f, 1.0f } } }, // right (at far plane)
        { { { -1.0f, 0.0f, 1.0f, 1.0f } } }, // left
        { { { 0.0f, 1.0f, 1.0f, 1.0f } } }, // top
        { { { 0.0f, -1.0f, 1.0f, 1.0f } } }, // bottom

        { { { 0.0f, 0.0f, 0.0f, 1.0f } } }, // near
        { { { 0.0f, 0.0f, 1.0f, 1.0f } } } // far
    };

    DirectX::XMVECTOR Determinant;
    DirectX::XMMATRIX matInverse = DirectX::XMMatrixInverse(&Determinant, camera_projection_matrix);

    DirectX::XMVECTOR Points[6];

    for (size_t i = 0; i < 6; ++i) {
        // Transform point.
        Points[i] = XMVector4Transform(HomogenousPoints[i], matInverse);
    }

    _origin = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    _orientation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    Points[0] = DirectX::XMVectorMultiply(Points[0], DirectX::XMVectorReciprocal(DirectX::XMVectorSplatZ(Points[0])));
    Points[1] = DirectX::XMVectorMultiply(Points[1], DirectX::XMVectorReciprocal(DirectX::XMVectorSplatZ(Points[1])));
    Points[2] = DirectX::XMVectorMultiply(Points[2], DirectX::XMVectorReciprocal(DirectX::XMVectorSplatZ(Points[2])));
    Points[3] = DirectX::XMVectorMultiply(Points[3], DirectX::XMVectorReciprocal(DirectX::XMVectorSplatZ(Points[3])));

    _rightSlope = DirectX::XMVectorGetX(Points[0]);
    _leftSlope = DirectX::XMVectorGetX(Points[1]);
    _topSlope = DirectX::XMVectorGetY(Points[2]);
    _bottomSlope = DirectX::XMVectorGetY(Points[3]);

    Points[4] = DirectX::XMVectorMultiply(Points[4], DirectX::XMVectorReciprocal(DirectX::XMVectorSplatW(Points[4])));
    Points[5] = DirectX::XMVectorMultiply(Points[5], DirectX::XMVectorReciprocal(DirectX::XMVectorSplatW(Points[5])));

    _nearZ = DirectX::XMVectorGetZ(Points[4]);
    _farZ = DirectX::XMVectorGetZ(Points[5]);
}

void FrustumCulling::Transform(FrustumCulling &local_f_culling, DirectX::XMMATRIX view_to_local) {
    DirectX::XMVECTOR vOrigin = DirectX::XMLoadFloat3(&_origin);
    DirectX::XMVECTOR vOrientation = DirectX::XMLoadFloat4(&_orientation);

    // Composite the frustum rotation and the transform rotation
    DirectX::XMMATRIX nM;
    nM.r[0] = DirectX::XMVector3Normalize(view_to_local.r[0]);
    nM.r[1] = DirectX::XMVector3Normalize(view_to_local.r[1]);
    nM.r[2] = DirectX::XMVector3Normalize(view_to_local.r[2]);
    nM.r[3] = DirectX::g_XMIdentityR3;
    DirectX::XMVECTOR Rotation = XMQuaternionRotationMatrix(nM);
    vOrientation = DirectX::XMQuaternionMultiply(vOrientation, Rotation);

    // Transform the center.
    vOrigin = DirectX::XMVector3Transform(vOrigin, view_to_local);
    // Store the frustum.
    DirectX::XMStoreFloat3(&local_f_culling._origin, vOrigin);
    DirectX::XMStoreFloat4(&local_f_culling._orientation, vOrientation);

    // Scale the near and far distances (the slopes remain the same).
    DirectX::XMVECTOR dX = DirectX::XMVector3Dot(view_to_local.r[0], view_to_local.r[0]);
    DirectX::XMVECTOR dY = DirectX::XMVector3Dot(view_to_local.r[1], view_to_local.r[1]);
    DirectX::XMVECTOR dZ = DirectX::XMVector3Dot(view_to_local.r[2], view_to_local.r[2]);

    DirectX::XMVECTOR d = DirectX::XMVectorMax(dX, DirectX::XMVectorMax(dY, dZ));
    float Scale = sqrtf(DirectX::XMVectorGetX(d));

    local_f_culling._nearZ = _nearZ * Scale;
    local_f_culling._farZ = _farZ * Scale;

    local_f_culling._rightSlope = _rightSlope;
    local_f_culling._leftSlope = _leftSlope;
    local_f_culling._topSlope = _topSlope;
    local_f_culling._bottomSlope = _bottomSlope;
}

bool FrustumCulling::ContainsByPositonPoint(DirectX::XMVECTOR position)
{
    DirectX::XMVECTOR vOrigin = DirectX::XMLoadFloat3(&_origin);
    DirectX::XMVECTOR vOrientation = DirectX::XMLoadFloat4(&_orientation);

    // Create 6 planes (do it inline to encourage use of registers)
    DirectX::XMVECTOR NearPlane = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, _nearZ);
    NearPlane = PlaneTransform(NearPlane, vOrientation, vOrigin); // Поменять так как реализация другая 
    NearPlane = DirectX::XMPlaneNormalize(NearPlane);

    DirectX::XMVECTOR FarPlane = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, -_farZ);
    FarPlane = PlaneTransform(FarPlane, vOrientation, vOrigin);
    FarPlane = DirectX::XMPlaneNormalize(FarPlane);

    DirectX::XMVECTOR RightPlane = DirectX::XMVectorSet(1.0f, 0.0f, -_rightSlope, 0.0f);
    RightPlane = PlaneTransform(RightPlane, vOrientation, vOrigin);
    RightPlane = DirectX::XMPlaneNormalize(RightPlane);

    DirectX::XMVECTOR LeftPlane = DirectX::XMVectorSet(-1.0f, 0.0f, _leftSlope, 0.0f);
    LeftPlane = PlaneTransform(LeftPlane, vOrientation, vOrigin);
    LeftPlane = DirectX::XMPlaneNormalize(LeftPlane);

    DirectX::XMVECTOR TopPlane = DirectX::XMVectorSet(0.0f, 1.0f, -_topSlope, 0.0f);
    TopPlane = PlaneTransform(TopPlane, vOrientation, vOrigin);
    TopPlane = DirectX::XMPlaneNormalize(TopPlane);

    DirectX::XMVECTOR BottomPlane = DirectX::XMVectorSet(0.0f, -1.0f, _bottomSlope, 0.0f);
    BottomPlane = PlaneTransform(BottomPlane, vOrientation, vOrigin);
    BottomPlane = DirectX::XMPlaneNormalize(BottomPlane);


    /*DirectX::XMVECTOR Extents = { 0, 0, 0 };
    DirectX::XMVECTOR Dist = DirectX::XMVector4Dot(position, NearPlane);
    DirectX::XMVECTOR Radius = DirectX::XMVector3Dot(Extents, DirectX::XMVectorAbs(NearPlane));*/

    return ContainedBy(NearPlane, FarPlane, RightPlane, LeftPlane, TopPlane, BottomPlane, position);
}

bool FrustumCulling::ContainedBy(DirectX::FXMVECTOR Plane0, DirectX::FXMVECTOR Plane1, DirectX::FXMVECTOR Plane2,
  DirectX::GXMVECTOR Plane3, DirectX::HXMVECTOR Plane4, DirectX::HXMVECTOR Plane5, DirectX::XMVECTOR position) {
    
    DirectX::XMVECTOR vCenter = position;
    DirectX::XMVECTOR vExtents = {0,0,0};

    vCenter = DirectX::XMVectorInsert<0, 0, 0, 0, 1>(vCenter, DirectX::XMVectorSplatOne());

    DirectX::XMVECTOR Outside, Inside;

    IntersectWithPlane(vCenter, vExtents, Plane0, Outside, Inside);

    DirectX::XMVECTOR AnyOutside = Outside;
    DirectX::XMVECTOR AllInside = Inside;

    IntersectWithPlane(vCenter, vExtents, Plane1, Outside, Inside);
    AnyOutside = DirectX::XMVectorOrInt(AnyOutside, Outside);
    AllInside = DirectX::XMVectorAndInt(AllInside, Inside);

    IntersectWithPlane(vCenter, vExtents, Plane2, Outside, Inside);
    AnyOutside = DirectX::XMVectorOrInt(AnyOutside, Outside);
    AllInside = DirectX::XMVectorAndInt(AllInside, Inside);

    IntersectWithPlane(vCenter, vExtents, Plane3, Outside, Inside);
    AnyOutside = DirectX::XMVectorOrInt(AnyOutside, Outside);
    AllInside = DirectX::XMVectorAndInt(AllInside, Inside);

    IntersectWithPlane(vCenter, vExtents, Plane4, Outside, Inside);
    AnyOutside = DirectX::XMVectorOrInt(AnyOutside, Outside);
    AllInside = DirectX::XMVectorAndInt(AllInside, Inside);

    IntersectWithPlane(vCenter, vExtents, Plane5, Outside, Inside);
    AnyOutside = DirectX::XMVectorOrInt(AnyOutside, Outside);
    AllInside = DirectX::XMVectorAndInt(AllInside, Inside);

    if (DirectX::XMVector4EqualInt(AnyOutside, DirectX::XMVectorTrueInt()))
        return false;

    if (DirectX::XMVector4EqualInt(AllInside, DirectX::XMVectorTrueInt()))
        return true;

    return true;
}

void FrustumCulling::IntersectWithPlane(DirectX::FXMVECTOR Center, DirectX::FXMVECTOR Extents, DirectX::FXMVECTOR Plane,
  DirectX::XMVECTOR& Outside, DirectX::XMVECTOR& Inside) {
    // Compute the distance to the center of the box.
    DirectX::XMVECTOR Dist = DirectX::XMVector4Dot(Center, Plane);

    // Project the axes of the box onto the normal of the plane.  Half the
    // length of the projection (sometime called the "radius") is equal to
    // h(u) * abs(n dot b(u))) + h(v) * abs(n dot b(v)) + h(w) * abs(n dot b(w))
    // where h(i) are extents of the box, n is the plane normal, and b(i) are the
    // axes of the box. In this case b(i) = [(1,0,0), (0,1,0), (0,0,1)].
    DirectX::XMVECTOR Radius = DirectX::XMVector3Dot(Extents, DirectX::XMVectorAbs(Plane));

    // Outside the plane?
    Outside = DirectX::XMVectorGreater(Dist, Radius);

    // Fully inside the plane?
    Inside = DirectX::XMVectorLess(Dist, DirectX::XMVectorNegate(Radius));
}

DirectX::XMVECTOR FrustumCulling::PlaneTransform(DirectX::XMVECTOR plane, DirectX::XMVECTOR rotation, DirectX::XMVECTOR translation) 
{
    DirectX::XMVECTOR vNormal = DirectX::XMVector3Rotate(plane, rotation);
    DirectX::XMVECTOR vD = DirectX::XMVectorSubtract(
        DirectX::XMVectorSplatW(plane), 
        DirectX::XMVector3Dot(vNormal, translation)
    );

    return DirectX::XMVectorInsert<0, 0, 0, 0, 1>(vNormal, vD);
}