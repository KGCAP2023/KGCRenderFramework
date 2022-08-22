#include "SimpleRenderableObject.h"

bool SimpleRenderableObject::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_1>& constantBuffer)
{
	this->deviceContext = deviceContext;
	this->constantBuffer = &constantBuffer;
	std::vector<SimpleVertex> vertices;

	vertices.push_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) });
	vertices.push_back(SimpleVertex{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });

	DWORD arr[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	std::vector<DWORD> indices(std::begin(arr),std::end(arr));

	std::vector<DWORD>& dd = indices;

	this->mesh = SimpleMesh(device,deviceContext,vertices,indices);

	this->SetPosition(2.0f, 2.0f, -5.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->Update();
	return true;
}

void printMatrix(const XMMATRIX & m)
{

	std::cout << "-----------------------" << std::endl;
	for (int i = 0; i < 4; ++i) {

		std::cout << XMVectorGetX(m.r[i]) << "\t"
		<< XMVectorGetY(m.r[i]) << "\t"
		<< XMVectorGetZ(m.r[i]) << "\t"
		<< XMVectorGetW(m.r[i]) << "\t"
		<< std::endl;
	}
	std::cout << "-----------------------" << std::endl;

}


//model mesh 합쳐져있음
void SimpleRenderableObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//상수버퍼 등록
	this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer->GetAddressOf());
	this->constantBuffer->data.WVP = XMMatrixTranspose(this->worldMatrix * viewProjectionMatrix);
	this->constantBuffer->ApplyChanges();
	this->mesh.Draw();
}

void SimpleRenderableObject::Update()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) * XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	this->UpdateDirection();
}
