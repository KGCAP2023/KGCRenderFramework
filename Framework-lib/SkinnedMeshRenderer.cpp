#include "pch.h"
#include "SkinnedMeshRenderer.h"
#include "ResourceManager.h"
#include "BoundingBox.h"
#include <time.h>


long long GetCurrentTimeMillis()
{
#ifdef _WIN32
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif
}

bool SkinnedMeshRenderer::SetSkinnedMesh(SkinnedMesh* model)
{
	if (model == nullptr)
		return false;
	this->model = model;
	owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
	owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
	owner->transform.SetScale(0.1f, 0.1f, 0.1f);
	this->Update();
	StartTimeMillis = GetCurrentTimeMillis();

	return true;
}

void SkinnedMeshRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{
	if (model != nullptr)
	{
		if (this->isStart)
		{
			long long CurrentTimeMillis = GetCurrentTimeMillis();
			float AnimationTimeSec = ((float)(CurrentTimeMillis - StartTimeMillis)) / 1000.0f;
			model->GetBoneTransforms(AnimationTimeSec, this->matrix,this->selectedAnimation);
		}
		model->Draw(owner->transform.worldMatrix, viewProjectionMatrix);
	}
}

void SkinnedMeshRenderer::Update()
{

}

void SkinnedMeshRenderer::InitBoundingBox()
{
	this->bbox = new BoundingBox3D(this->owner, this, res);
	this->owner->bbox = this->bbox;
}

BoundingBoxRenderer* SkinnedMeshRenderer::GetBoundingBox()
{
	return this->bbox;
}

std::string SkinnedMeshRenderer::GetName()
{
	if(this->model != nullptr)
		return this->model->GetName();
	else
		return std::string();
}

std::string SkinnedMeshRenderer::GetPath()
{
	return this->model->GetPath();
}

const aiScene* SkinnedMeshRenderer::GetAiScene()
{
	return this->model->GetAiScene();
}
