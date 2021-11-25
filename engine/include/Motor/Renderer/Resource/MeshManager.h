#pragma once

#include <Motor/Renderer/Resource/ResourceManager.h>
#include <Motor/Renderer/Resource/Mesh.h>

namespace motor {
class MeshManager : public ResourceManager {
public:
    MeshManager();
    ~MeshManager();

    MeshPtr createMeshFromFile(const wchar_t* file_path);
    MeshPtr createMesh(
      VertexMesh* vertex_list_data, unsigned int vertex_list_size,
      unsigned int* index_list_data, unsigned int index_list_size,
      MaterialSlot* material_slot_list, unsigned int material_slot_list_size);

protected:
    virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};

} // namespace motor