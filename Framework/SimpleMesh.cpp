#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(ID3D11Device* device,
    ID3D11DeviceContext* deviceContext,
    std::vector<SimpleVertex>& vertices,
    std::vector<DWORD>& indices)
{
    this->deviceContext = deviceContext;
    this->textures = textures;

    HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);

    if (FAILED(hr)) Logger::Log(hr, L"[X] 버텍스버퍼 로드 실패");

    hr = this->indexbuffer.Init(indices.data(), indices.size(), device);

    if (FAILED(hr)) Logger::Log(hr, L"[X] 인덱스버퍼 로드 실패");
}


SimpleMesh::SimpleMesh(const SimpleMesh& mesh)
{
    this->deviceContext = mesh.deviceContext;
    this->indexbuffer = mesh.indexbuffer;
    this->vertexbuffer = mesh.vertexbuffer;
    this->textures = mesh.textures;
}

void SimpleMesh::Draw()
{
    UINT offset = 0;
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
    this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
}
