#include "pch.h"
#include "BoundingBoxRenderer.h"


void BoundingBoxRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
    if (isActive)
    {
        UINT offset = 0;
        this->deviceContext->PSSetShader(ps->GetShader(), NULL, 0);
        this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
        this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
        this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

}

void BoundingBoxRenderer::Update()
{

}

