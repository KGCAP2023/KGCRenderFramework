#pragma once
#include "Gameobject.h"
#include "Sprite.h"
#include "Animation2D.h"
#include "Renderer.h"

/// <summary>
/// 2D스프라이트를 가져옵니다.
/// </summary>
class SpriteRenderer : public Component , public Renderer
{
public:
	SpriteRenderer(GameObject* owner, ResourceManager* res) : Component(owner)
	{
		this->type = Component::Type::RENDERER_SPRITE;
		this->name = "SpriteRenderer";
		this->isSpriteRender = true;
		this->res = res;
	}

	virtual void Update() override;
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);

	virtual void InitBoundingBox() override;
	virtual BoundingBoxRenderer* GetBoundingBox() override;

	bool SetSprite(Sprite* sprite)
	{
		if (sprite != nullptr)
		{
			this->sprite = sprite;
			this->spriteBatch = sprite->GetSpriteBatch();
			this->InitBoundingBox();
			return true;
		}
		else
			return false;
	}

	Sprite* GetSprite() { return this->sprite; };

	//void Draw(const DirectX::SimpleMath::Vector2& pos, SpriteBatch* spriteBatch);

	void AddAnimation2D(const std::string& name, int x, int y, int width, int height, int count,
		float holdTime, XMVECTORF32 chroma)
	{
		Animation2D* animation = new Animation2D(x, y, width, height, count,
			*sprite, holdTime, chroma);
		this->_AnimationMap.insert(std::make_pair<>(name, animation));
	};

	void RemoveAnimation2D(const std::string& name)
	{
		this->_AnimationMap.erase(name);
	};

	Animation2D* FindAnimation2D(const std::string& key)
	{
		if (this->_AnimationMap.find(key) != this->_AnimationMap.end()) {
			return this->_AnimationMap[key];
		}
		return nullptr;
	}

	bool SelectAnimation(const std::string& name)
	{
		Animation2D* selected = FindAnimation2D(name);

		if (selected != nullptr)
		{
			selectedAnimation = selected;
			return true;
		}
		else
			return false;
	}

	Animation2D* GetSelectedAnim() { return selectedAnimation; }

private:
	Sprite* sprite = nullptr;
	SpriteBatch* spriteBatch = nullptr;
	Animation2D* selectedAnimation = nullptr;
	std::unordered_map<std::string, Animation2D*> _AnimationMap;

	BoundingBoxRenderer* bbox = nullptr;
	ResourceManager* res = nullptr;
};