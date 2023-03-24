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

	//버텍스를 정의합니다.
	//<주의> 
	// 버텍스 쉐이더에 들어가는 구조체(Vertex.h)와 동일해야합니다

	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });

	//정점에대한 인덱스를 정의합니다. 
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

	//버텍스 버퍼와 인덱스 버퍼에 등록합니다.
	HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);
	hr = this->indexbuffer.Init(indices.data(), indices.size(), device);

}

void BoundingBox3D::Draw(const XMMATRIX& viewProjectionMatrix)
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
