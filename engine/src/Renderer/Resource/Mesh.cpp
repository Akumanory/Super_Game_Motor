#include <Motor/Renderer/Resource/Mesh.h>

#include <Motor/Renderer/RHI/GraphicsEngine.h>
#include <Motor/Renderer/RHI/VertexMesh.h>

#include <filesystem>
//#include <locale>
//#include <codecvt>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <SimpleMath.h>

using namespace motor;

using namespace DirectX;
using namespace DirectX::SimpleMath;

Mesh::Mesh(const wchar_t* full_path)
    : Resource(full_path) {
    auto directory = StringHelper::GetDirectoryFromPath(filePath);

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
    if (pScene == nullptr)
        return false;

    // Bounding box min max vertex params
    XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
    XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

    vMin = XMLoadFloat3(&vMinf3);
    vMax = XMLoadFloat3(&vMaxf3);

    this->ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

    XMStoreFloat3(&bounding_box.Center, 0.5f * (vMin + vMax));
    XMStoreFloat3(&bounding_box.Extents, 0.5f * (vMax - vMin));

    // Data to fill
    std::vector<VertexMesh> list_vertices;
    std::vector<unsigned int> list_indices;

    size_t vertex_buffer_size = 0;

    //Get vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        XMVECTOR P = XMLoadFloat3(&vertex.pos);

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);

        vMin = XMVectorMin(vMin, P);
        vMax = XMVectorMax(vMax, P);
    }

    //Get indices
    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::vector<Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
}

Mesh::Mesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size,
  unsigned int* index_list_data, unsigned int index_list_size,
  MaterialSlot* material_slot_list, unsigned int material_slot_list_size)
    : Resource(L"") {
    void* shader_byte_code = nullptr;
    size_t size_shader = 0;
    GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
    m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list_data,
      sizeof(VertexMesh),
      (UINT)vertex_list_size, shader_byte_code, (UINT)size_shader);
    m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list_data,
      (UINT)index_list_size);

    m_mat_slots.resize(material_slot_list_size);

    for (unsigned int i = 0; i < material_slot_list_size; i++) {
        m_mat_slots[i] = material_slot_list[i];
    }
}

Mesh::~Mesh() {
}

const VertexBufferPtr& Mesh::getVertexBuffer() {
    return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer() {
    return m_index_buffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot) {
    return m_mat_slots[slot];
}

size_t Mesh::getNumMaterialSlots() {
    return m_mat_slots.size();
}

void Mesh::computeTangents(const Vector3& v0,
  const Vector3& v1,
  const Vector3& v2,
  const Vector2& t0,
  const Vector2& t1,
  const Vector2& t2,
  Vector3& tangent, Vector3& binormal) {
    Vector3 deltaPos1 = v1 - v0;
    Vector3 deltaPos2 = v2 - v0;

    Vector2 deltaUV1 = t1 - t0;
    Vector2 deltaUV2 = t2 - t0;

    float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
    tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y);
    tangent = Vector3::normalize(tangent);
    binormal = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x);
    binormal = Vector3::normalize(binormal);
}
