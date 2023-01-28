#include "pch.h"
#include "SkinnedMeshRenderer.h"
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

bool SkinnedMeshRenderer::Init(
	const std::string& filePath,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<CB_VS_2>& cb_vs_vertexshader,
	ConstantBuffer<CB_VS_3>& cb_vs_vertexshader2,
	VertexShader* vertexShader,
	PixelShader* pixelShader)
{
	if (!model.LoadMesh(filePath, device, deviceContext, cb_vs_vertexshader, cb_vs_vertexshader2, vertexShader, pixelShader))
		return false;


	owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
	owner->transform.SetRotation(0.0f, 0.0f, 0.0f);
	owner->transform.SetScale(1.0f, 1.0f, 1.0f);
	this->Update();
	StartTimeMillis = GetCurrentTimeMillis();

	return true;
}

void SkinnedMeshRenderer::Draw(const XMMATRIX& viewProjectionMatrix)
{

	if (this->isStart)
	{

		long long CurrentTimeMillis = GetCurrentTimeMillis();
		float AnimationTimeSec = ((float)(CurrentTimeMillis - StartTimeMillis)) / 1000.0f;
		model.GetBoneTransforms(AnimationTimeSec, this->matrix);

	}
	else
	{

	}
	model.Draw(owner->transform.worldMatrix, viewProjectionMatrix);
}

void SkinnedMeshRenderer::Update()
{
	GameObject* owner = this->owner;
	owner->transform.worldMatrix = DirectX::XMMatrixScaling(owner->transform.scale.x, owner->transform.scale.y, owner->transform.scale.z) * XMMatrixRotationRollPitchYaw(owner->transform.rotation.x, owner->transform.rotation.y, owner->transform.rotation.z) * XMMatrixTranslation(owner->transform.position.x, owner->transform.position.y, owner->transform.position.z);
	owner->transform.UpdateDirection();

}

std::string SkinnedMeshRenderer::GetPath()
{
	return this->model.GetPath();
}

const aiScene* SkinnedMeshRenderer::GetAiScene()
{
	return this->model.GetAiScene();
}
