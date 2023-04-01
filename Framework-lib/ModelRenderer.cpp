#include "pch.h"
#include "ModelRenderer.h"


bool ModelRenderer::SetModel(Model *model)
{
	if (model == nullptr)
		return false;
	this->model = model;
	owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
	owner->transform.SetScale(1.0f, 1.0f, 1.0f);
	this->Update();
	return true;
}

void ModelRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
	if(model != nullptr)
		model->Draw(owner->transform.worldMatrix,viewProjectionMatrix);
}

void ModelRenderer::Update()
{
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = DirectX::XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, owner->transform.scale.z) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x, owner->transform.position.y, owner->transform.position.z);
	owner->transform.UpdateDirection();
}

std::vector<Mesh>& ModelRenderer::GetMeshes()
{
	return this->model->GetMeshes();
}

std::string ModelRenderer::GetPath()
{
	return this->model->GetPath();
}

//const aiScene* ModelRenderer::GetAiScene()
//{
//	return this->model.getAiScene();
//}
