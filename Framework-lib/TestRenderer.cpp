#include "pch.h"
#include "TestRenderer.h"

bool TestRenderer::Init(ResourceManager* res, Sprite *sprite)
{
    //�⺻���� 
    this->device = res->device.Get();
    this->deviceContext = res->deviceContext.Get();
    this->constantBuffer = res->cb2;
    this->pixelShader = res->FindPixelShader("ps_2");
    this->vertexShader = res->FindVertexShader("vs_2");
    this->sprite = sprite;

    //���� ��ǥ �׸���
	vertices.push_back(Vertex3D(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex3D(0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex3D(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex3D(0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
    
    //�ݵ�� �ð�������� �׸���!!!!
    std::vector<DWORD> arr =
    {
        0, 2, 1,
        2, 3, 1
    };

    //�̰Ŵ� �ݽð����
    //�ݽð�������� �׸��� ���鿡�� �Ⱥ��̰� ���ʿ��� �ٶ󺸸� ���Դϴ�.
    //std::vector<DWORD> arr =
    //{
    //    0, 1, 2,
    //    2, 1, 3
    //};

	std::vector<DWORD> indices(std::begin(arr), std::end(arr));

	//���ؽ� ���ۿ� �ε��� ���ۿ� ����մϴ�.
	HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);
	hr = this->indexbuffer.Init(indices.data(), indices.size(), device);



    //<�ʼ�> �ʱ�ȭ �۾� �� ���ٰ� ���ϸ� scale �⺻�� 0 0 �̶� �Ⱥ���

    this->owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
    this->owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
    this->owner->transform.SetScale(1.f, 1.f);

	return true;
}

void TestRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
    // ���ؽ� ���̴��� ���� ����ü�� ���ؽ� ���̾ƿ��� �����ؾ��մϴ�.
    // ���ؽ� ���̾ƿ��� GraphicManager.cpp�� InitializeShaders()�� �����Ͻʽÿ�
    this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->deviceContext->IASetInputLayout(this->vertexShader->GetInputLayout());

    //���ؽ� ���̴��� �����մϴ�.
    this->deviceContext->VSSetShader(this->vertexShader->GetShader(), NULL, 0);
    //�ȼ� ���̴��� �����մϴ�.
    this->deviceContext->PSSetShader(this->pixelShader->GetShader(), NULL, 0);

    XMMATRIX worldMatrix = this->owner->transform.worldMatrix;

    //������� - ���̴����� WVP���� ������ �����ְ� ���� �������ݴϴ�. 
    {
        //������۸� �����մϴ�.
        this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());
        //������ۿ� ���� ���� �����մϴ�.
        this->constantBuffer.data.WVP = XMMatrixTranspose(worldMatrix * viewProjectionMatrix); //Calculate World-View-Projection Matrix
        this->constantBuffer.data.W = XMMatrixTranspose(worldMatrix);
        //�ش� ���� �����ŵ�ϴ�.
        this->constantBuffer.ApplyChanges();
    }

    UINT offset = 0;
    
    if(this->sprite != nullptr)
    this->deviceContext->PSSetShaderResources(0,1,this->sprite->GetTextureResourceViewAddress());

    //���ؽ� ���۸� �����մϴ�.
    this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
    //�ε��� ���۸� �����մϴ�.
    this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //���������ο� ���������� ����մϴ�.
    this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);

}

void TestRenderer::Update()
{
    //��� ������Ʈ
    //Scale * Rotate * Translation
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = DirectX::XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, owner->transform.scale.z) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x, owner->transform.position.y, owner->transform.position.z);
	owner->transform.UpdateDirection();
}
