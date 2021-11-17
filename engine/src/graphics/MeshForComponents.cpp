#include <motor/graphics/MeshForComponents.h>

MeshForComponents::MeshForComponents(ID3D11Device* device, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix) {
    _textures = textures;
    _transformMatrix = transformMatrix;

    HRESULT hr = _vertexbuffer.Initialize(device, vertices.data(), vertices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

    hr = _indexbuffer.Initialize(device, indices.data(), indices.size());
    COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

MeshForComponents::MeshForComponents(const MeshForComponents& mesh) 
{
    _indexbuffer = mesh._indexbuffer;
    _vertexbuffer = mesh._vertexbuffer;
    _textures = mesh._textures;
    _transformMatrix = mesh._transformMatrix;
}

const DirectX::XMMATRIX& MeshForComponents::GetMeshTransform() 
{
    return _transformMatrix;
}

const std::vector<Texture>& MeshForComponents::GetTextures() 
{
    return _textures;
}

const VertexBuffer<Vertex>& MeshForComponents::GetVertexBuffer() 
{
    return _vertexbuffer;
}

const IndexBuffer& MeshForComponents::GetIndexBuffer() 
{ 
    return _indexbuffer;
}