#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "Renderer.h"
#include "SkinnedMesh.h"
#include "ResourceManager.h"

/// <summary>
/// 애니메이션이들어있는 동적 3D모델을 로드할때 사용하는 컴포넌트 입니다. 
/// </summary>
class SkinnedMeshRenderer : public Component , public Renderer
{
public:

	SkinnedMeshRenderer(GameObject* owner) : Component(owner)
	{
		this->type = Component::Type::RENDERER_SKINNED_MODEL;
		this->name = "SkinnedMeshRenderer";
	}

	bool SetSkinnedMesh(SkinnedMesh* model);

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

	std::string GetPath();
	const aiScene* GetAiScene();

	bool AddAnimation(const std::string filepath)
	{
		if (model != nullptr)
			return this->model->AddAnimation(filepath);
		else
			return false;
	}

	void AnimationForeach(std::function<void(Animation3D*)> callback)
	{
		if (model != nullptr)
		{
			for (Animation3D* anim : *model->GetAnimationList())
			{
				callback(anim);
			}
		}
	}

	int GetAnimationSize()
	{
		if (model != nullptr)
			return model->GetAnimationList()->size();
		else
			return 0;
	}

	std::vector<Animation3D*>* GetAnimationList()
	{
		if (model != nullptr)
			return model->GetAnimationList();
		else
			return nullptr;
	}


	void SetAnimation(int index)
	{
		if(index < GetAnimationSize() && model != nullptr)
			selectedAnimation = model->GetAnimationList()->at(index);
	}

	void PlayAnimation()
	{
		isStart = true;
	}

	void PauseAnimation()
	{
		isStart = false;
	}

	void StopAnimation()
	{
		isStart = false;
		StartTimeMillis = 0;
		if (model != nullptr)
		{
			model->InitDefaultPose();
		}
	}

	bool isStart = false;
private:
	SkinnedMesh* model = nullptr;
	Animation3D* selectedAnimation = nullptr;
	std::vector<XMMATRIX> matrix;
	long long StartTimeMillis = 0;
};