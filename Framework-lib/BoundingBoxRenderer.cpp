#include "pch.h"
#include "BoundingBoxRenderer.h"


void BoundingBoxRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
    if (isActive)
    {
        // ���ؽ� ���̴��� ���� ����ü�� ���ؽ� ���̾ƿ��� �����ؾ��մϴ�.
        // ���ؽ� ���̾ƿ��� GraphicManager.cpp�� InitializeShaders()�� �����Ͻʽÿ�
        deviceContext->IASetInputLayout(this->vs->GetInputLayout());

        //���ؽ� ���̴��� �����մϴ�.
        this->deviceContext->VSSetShader(this->vs->GetShader(), NULL, 0);
        //�ȼ� ���̴��� �����մϴ�.
        this->deviceContext->PSSetShader(this->ps->GetShader(), NULL, 0);

        //������� - ���̴����� WVP���� ������ �����ְ� ���� �������ݴϴ�. 
        {
            //������۸� �����մϴ�.
            this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer->GetAddressOf());
            //������ۿ� ���� ���� �����մϴ�.
            this->constantBuffer->data.WVP = XMMatrixTranspose(this->owner->transform.worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
            //�ش� ���� �����ŵ�ϴ�.
            this->constantBuffer->ApplyChanges();
        }

        

        UINT offset = 0; 
        //������ �׸��� ����� �����մϴ�.
        //�⺻�� D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST�� ����մϴ�.
        //�����ڵ�� ��谪�� �׸������� D3D_PRIMITIVE_TOPOLOGY_LINELIST�� ����ϸ�
        //D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST ���� ���� �Է°� �ε��� �Է��� �ﰢ�� �׸��� �������� ���� �Ͻʽÿ�

        this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

        //���ؽ� ���۸� �����մϴ�.
        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
        //�ε��� ���۸� �����մϴ�.
        this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        //���������ο� ���������� ����մϴ�.
        this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
        
    }

}

void BoundingBoxRenderer::Update()
{

}

