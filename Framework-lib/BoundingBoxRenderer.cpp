#include "pch.h"
#include "BoundingBoxRenderer.h"


void BoundingBoxRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
    if (isActive)
    {
        // 버텍스 쉐이더에 들어가는 구조체는 버텍스 레이아웃과 동일해야합니다.
        // 버텍스 레이아웃은 GraphicManager.cpp의 InitializeShaders()를 참조하십시요
        deviceContext->IASetInputLayout(this->vs->GetInputLayout());

        //버텍스 쉐이더를 셋팅합니다.
        this->deviceContext->VSSetShader(this->vs->GetShader(), NULL, 0);
        //픽셀 쉐이더를 셋팅합니다.
        this->deviceContext->PSSetShader(this->ps->GetShader(), NULL, 0);

        //상수버퍼 - 쉐이더에서 WVP값을 가져다 쓸수있게 값을 셋팅해줍니다. 
        {
            //상수버퍼를 셋팅합니다.
            this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer->GetAddressOf());
            //상수버퍼에 들어가는 값을 셋팅합니다.
            this->constantBuffer->data.WVP = XMMatrixTranspose(this->owner->transform.worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
            //해당 값을 적용시킵니다.
            this->constantBuffer->ApplyChanges();
        }

        

        UINT offset = 0; 
        //정점을 그리는 방식을 결정합니다.
        //기본은 D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST를 사용합니다.
        //현재코드는 경계값을 그리기위해 D3D_PRIMITIVE_TOPOLOGY_LINELIST를 사용하며
        //D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST 사용시 정점 입력과 인덱스 입력을 삼각형 그리는 방향으로 정의 하십시요

        this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

        //버텍스 버퍼를 셋팅합니다.
        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
        //인덱스 버퍼를 셋팅합니다.
        this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        //파이프라인에 최종적으로 등록합니다.
        this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
        
    }

}

void BoundingBoxRenderer::Update()
{

}

