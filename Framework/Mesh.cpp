#include "Mesh.h"



Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex3D>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix)
{
    this->deviceContext = deviceContext;
    this->textures = textures;
    this->transformMatrix = transformMatrix;
    HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);

    if (FAILED(hr))
    Logger::Log(hr,L"[X] 버텍스버퍼 로드 실패");

    hr = this->indexbuffer.Init(indices.data(), indices.size(),device);

    if (FAILED(hr))
    Logger::Log(hr, L"[X] 인덱스버퍼 로드 실패");

}

Mesh::Mesh(const Mesh& mesh)
{
    this->deviceContext = mesh.deviceContext;
    this->indexbuffer = mesh.indexbuffer;
    this->vertexbuffer = mesh.vertexbuffer;
    this->textures = mesh.textures;
    this->transformMatrix = mesh.transformMatrix;
}

void Mesh::Draw()
{
    UINT offset = 0;

    for (int i = 0; i < textures.size(); i++)
    {
        //if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
        //{
            this->deviceContext->PSSetShaderResources(0, 1, textures[i].GetTextureResourceViewAddress());
        //    break;
        //}
    }

    this->deviceContext->IASetVertexBuffers(0,1,this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
    this->deviceContext->IASetIndexBuffer(indexbuffer.Get(),DXGI_FORMAT_R32_UINT,0);
    this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);

}

const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
    return  this->transformMatrix;
}

void Mesh::SetTransformMatrix(const DirectX::XMMATRIX& m)
{
    this->transformMatrix = m;
}
