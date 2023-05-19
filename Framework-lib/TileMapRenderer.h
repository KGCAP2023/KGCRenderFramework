#pragma once
#include "pch.h"
#include "Gameobject.h"
#include "TileMap.h"
#include "Renderer.h"

class TileMapRenderer : public Component , public Renderer
{
public:

	TileMapRenderer(GameObject* owner, ResourceManager* res);

	TileMapRenderer(const TileMapRenderer& rhs, GameObject* owner) : Component(owner)
	{
		std::cout << "[=] TileMapRenderer CLONE Process - Copy constructor called" << std::endl;
		this->type = rhs.type;
		this->name = rhs.name;
		this->tileMap = rhs.tileMap;
		this->bbox = rhs.bbox;
		this->res = rhs.res;
		this->isSpriteRender = rhs.isSpriteRender;
	}

	~TileMapRenderer()
	{
		std::cout << "[=] TileMapRenderer destructor called" << std::endl;
		tileMap = nullptr;
		bbox = nullptr;
		res = nullptr;
	}

	virtual Component* Copy(GameObject* owner)
	{
		Component* compo = new TileMapRenderer(*this, owner);
		return compo;
	};

	void SetTileMap(TileMap* tileMap)
	{
		this->tileMap = tileMap;
		InitBoundingBox();
	}

	TileMap* GetTileMap() { return this->tileMap; };
	void SetLayerDepth(float layer) { this->layer = layer; }
	float GetLayerDepth() { return this->layer; }

	virtual void InitBoundingBox() override;
	virtual BoundingBoxRenderer* GetBoundingBox() override;

	virtual void Update()
	{

	}

	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
	{
		if (tileMap != nullptr)
		{
			this->tileMap->Draw(viewProjectionMatrix, this->GetOwner()->transform, layer);
			this->bbox->SetLayerDepth(layer);
			this->bbox->Draw(viewProjectionMatrix);
		}
	}

private:
	TileMap* tileMap = nullptr;
	BoundingBox2D* bbox = nullptr;
	ResourceManager* res = nullptr;
	float layer = 0.0f;
};