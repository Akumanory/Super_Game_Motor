#pragma once

#include <Motor/Renderer/Resource/Resource.h>
#include <Motor/Renderer/RHI/VertexBuffer.h>
#include <Motor/Renderer/RHI/IndexBuffer.h>
#include <Motor/Renderer/RHI/VertexMesh.h>

#include <vector>
#include <SimpleMath.h>

namespace motor {

using namespace DirectX::SimpleMath;

struct MaterialSlot {
    size_t start_index = 0;
    size_t num_indices = 0;
    size_t material_id = 0;
};

class Mesh : public Resource {
public:
    Mesh(const wchar_t* full_path);
    Mesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size,
      unsigned int* index_list_data, unsigned int index_list_size,
      MaterialSlot* material_slot_list, unsigned int material_slot_list_size);
    ~Mesh();
    const VertexBufferPtr& getVertexBuffer();
    const IndexBufferPtr& getIndexBuffer();

    const MaterialSlot& getMaterialSlot(unsigned int slot);
    size_t getNumMaterialSlots();

private:
    void computeTangents(
      const Vector3& v0, const Vector3& v1, const Vector3& v2,
      const Vector2& t0, const Vector2& t1, const Vector2& t2,
      Vector3& tangent, Vector3& binormal);

private:
    VertexBufferPtr m_vertex_buffer;
    IndexBufferPtr m_index_buffer;
    std::vector<MaterialSlot> m_mat_slots;

private:
    friend class DeviceContext;
};
} // namespace motor
