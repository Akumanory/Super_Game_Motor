#pragma once

#include <SimpleMath.h>

namespace motor {

using namespace DirectX::SimpleMath;

class VertexMesh {
public:
    VertexMesh()
        : m_position()
        , m_texcoord()
        , m_normal() {
    }
    VertexMesh(const Vector3& position, const Vector2& texcoord, const Vector3& normal,
      const Vector3D& tangent, const Vector3& binormal)
        : m_position(position)
        , m_texcoord(texcoord)
        , m_normal(normal)
        , m_tangent(tangent)
        , m_binormal(binormal) {
    }
    VertexMesh(const VertexMesh& vertex)
        : m_position(vertex.m_position)
        , m_texcoord(vertex.m_texcoord)
        , m_normal(vertex.m_normal)
        , m_tangent(vertex.m_tangent)
        , m_binormal(vertex.m_binormal) {
    }

    ~VertexMesh() {
    }

public:
    Vector3 m_position;
    Vector2 m_texcoord;
    Vector3 m_normal;
    Vector3 m_tangent;
    Vector3 m_binormal;
};
} // namespace motor