#include "pch.h"
#include "ModelRenderer.h"
#include "ResourceManager.h"
#include "BoundingBox.h"

bool ModelRenderer::SetModel(Model *model)
{
	if (model == nullptr)
		return false;
	this->model = model;
	owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
	owner->transform.SetScale(1.0f, 1.0f, 1.0f);
	this->Update();
	this->InitBoundingBox();
	return true;
}

void ModelRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
	if (model != nullptr)
	{
		this->bbox->Draw(viewProjectionMatrix);
		this->model->Draw(owner->transform.worldMatrix, viewProjectionMatrix);
	}
		
}

void ModelRenderer::Update()
{

}

void ModelRenderer::InitBoundingBox()
{
	this->bbox = new BoundingBox3D(this->owner, this, this->res);
	this->owner->bbox = this->bbox;
}

BoundingBoxRenderer* ModelRenderer::GetBoundingBox()
{
	return this->bbox;
}

std::string ModelRenderer::GetName()
{
	return this->model->GetName();
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
