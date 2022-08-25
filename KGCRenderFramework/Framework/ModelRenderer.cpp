#include "ModelRenderer.h"

bool ModelRenderer::Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader)
{
	if (!model.Init(filePath, device, deviceContext, cb_vs_vertexshader))
		return false;
	owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
	owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
	owner->transform.SetScale(1.0f, 1.0f, 1.0f);
	this->Update();
	return true;
}

void ModelRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
	model.Draw(owner->transform.worldMatrix,viewProjectionMatrix);
}

void ModelRenderer::Update()
{
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = DirectX::XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, owner->transform.scale.z) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x, owner->transform.position.y, owner->transform.position.z);
	owner->transform.UpdateDirection();
}
