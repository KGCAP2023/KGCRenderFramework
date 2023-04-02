#include "pch.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include "ResourceManager.h"

#pragma region �׸���_����_���_����

	//ť��
	DWORD cubeArr[] =
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
	std::vector<DWORD> cube(std::begin(cubeArr), std::end(cubeArr));

	//�𷡽ð�
	DWORD hourGlassArr[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0,5,
		1,4,
		3,6,
		2,7,
	};
	std::vector<DWORD> hourGlass(std::begin(hourGlassArr), std::end(hourGlassArr));

#pragma endregion




BoundingBox3D::BoundingBox3D(GameObject* owner, ResourceManager* res) : BoundingBoxRenderer(owner)
{
	this->type = Component::Type::BOUNDING_BOX;
	this->deviceContext = res->deviceContext;
	this->ps = res->FindPixelShader("ps_1");
	this->vs = res->FindVertexShader("vs_1");
	this->constantBuffer = &res->cb1;

	XMMATRIX worldMatrix = owner->transform.worldMatrix;

	Renderer* render = nullptr;
	Component* com = owner->GetComponent(Component::Type::RENDERER_SKINNED_MODEL);

	if (com != nullptr)
	{
		min = { 0,0,0 };
		max = { 0,0,0 };
		render = dynamic_cast<Renderer*>(this->owner->GetComponent(Component::Type::RENDERER_SKINNED_MODEL));
	}
	else
	{
		com = owner->GetComponent(Component::Type::RENDERER_MODEL);

		if (com != nullptr)
		{
			min = { 0,0,0 };
			max = { 0,0,0 };
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

	//���ݴ� �������� ��� ã�ƺ���
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });


	//�ʱ⼳���� cube
	indices = cube;

	//���������� �ε����� �����մϴ�. 
	DrawSetting(res);
	
}


void BoundingBox3D::DrawSetting(ResourceManager* res)
{
	
	//���ؽ� ���ۿ� �ε��� ���ۿ� ����մϴ�.
	HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), res->device.Get());
	hr = this->indexbuffer.Init(indices.data(), indices.size(), res->device.Get());

}



void BoundingBox3D::ChangeDrawShape(ShapeType _type,  ResourceManager* res)
{

	switch (_type)
	{
	case BoundingBox3D::ShapeType::CUBE:
		indices = cube;
		break;
	case BoundingBox3D::ShapeType::HOUR_GLASS:
		indices = hourGlass;
		break;
	default:
		break;
	}

	DrawSetting(res);

}


void BoundingBox3D::Draw(const XMMATRIX& viewProjectionMatrix)
{

    if (isActive)
    {
        // ���ؽ� ���̴��� ���� ����ü�� ���ؽ� ���̾ƿ��� �����ؾ��մϴ�.
        // ���ؽ� ���̾ƿ��� GraphicManager.cpp�� InitializeShaders()�� �����Ͻʽÿ�

		this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
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

        

        //���ؽ� ���۸� �����մϴ�.
        this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
        //�ε��� ���۸� �����մϴ�.
        this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        //���������ο� ���������� ����մϴ�.
        this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
  
    }
}

void BoundingBox3D::Update()
{
	owner->transform.worldMatrix = DirectX::XMMatrixScaling(1, 1, 1) *
		XMMatrixRotationRollPitchYaw(0, 0, 0) *
		XMMatrixTranslation(owner->transform.position.x, owner->transform.position.y, owner->transform.position.z);
}
