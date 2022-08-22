#include "RenderableObject.h"

bool RenderableObject::Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader)
{
	if (!model.Init(filePath, device, deviceContext, cb_vs_vertexshader))
		return false;
	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->SetScale(1.0f, 1.0f, 1.0f);
	this->Update();
	return true;

}

void RenderableObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	
	model.Draw(this->worldMatrix, viewProjectionMatrix);
}

void RenderableObject::Update()
{
	this->worldMatrix = DirectX::XMMatrixScaling(this->scale.x,this->scale.y,this->scale.z) * XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) * XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	this->UpdateDirection();
}
