#pragma once

#include <motor/graphics/Vertex.h>
#include <motor/graphics/VertexBuffer.h>
#include <motor/graphics/IndexBuffer.h>
#include <motor/graphics/Texture.h>
#include <motor/other/COMException.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class MeshForComponents 
{
public:
    MeshForComponents(ID3D11Device* device, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix);
    MeshForComponents(const MeshForComponents& mesh);

    const DirectX::XMMATRIX& GetMeshTransform();
    const std::vector<Texture>& GetTextures();
    const VertexBuffer<Vertex>& GetVertexBuffer();
    const IndexBuffer& GetIndexBuffer();

private:
    VertexBuffer<Vertex> _vertexbuffer;
    IndexBuffer _indexbuffer;
    std::vector<Texture> _textures;

    DirectX::XMMATRIX _transformMatrix;
};