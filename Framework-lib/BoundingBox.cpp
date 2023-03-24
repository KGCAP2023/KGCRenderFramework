#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"

BoundingBox3D::BoundingBox3D(GameObject* owner, ID3D11Device* device, ID3D11DeviceContext* deviceContext, PixelShader* ps, VertexShader* vs, ConstantBuffer<CB_VS_1>& constantBuffer) : BoundingBoxRenderer(owner)
{
	this->type = Component::Type::BOUNDING_BOX;
	this->deviceContext = deviceContext;
	this->ps = ps;
	this->vs = vs;
	this->constantBuffer = &constantBuffer;

	min = { 0,0,0 };
	max = { 0,0,0 };

	XMMATRIX worldMatrix = owner->transform.worldMatrix;

	Renderer* render = nullptr;
	Component* com = owner->GetComponent(Component::Type::RENDERER_SKINNED_MODEL);

	if (com != nullptr)
	{
		render = dynamic_cast<Renderer*>(this->owner->GetComponent(Component::Type::RENDERER_SKINNED_MODEL));
	}
	else
	{
		com = owner->GetComponent(Component::Type::RENDERER_MODEL);

		if (com != nullptr)
		{
			render = dynamic_cast<Renderer*>(this->owner->GetComponent(Component::Type::RENDERER_MODEL));
		}
	}

	if (render != nullptr)
	{
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(render->GetPath(),
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded);

		processNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
	}

	//���ؽ��� �����մϴ�.
	//<����> 
	// ���ؽ� ���̴��� ���� ����ü(Vertex.h)�� �����ؾ��մϴ�

	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });

	//���������� �ε����� �����մϴ�. 
	DWORD arr[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	std::vector<DWORD> indices(std::begin(arr), std::end(arr));

	//���ؽ� ���ۿ� �ε��� ���ۿ� ����մϴ�.
	HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);
	hr = this->indexbuffer.Init(indices.data(), indices.size(), device);

}

void BoundingBox3D::Draw(const XMMATRIX& viewProjectionMatrix)
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
