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
		tileMap = rhs.tileMap;
		bbox = rhs.bbox;
		res = rhs.res;
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

	virtual void InitBoundingBox() override;
	virtual BoundingBoxRenderer* GetBoundingBox() override;

	virtual void Update()
	{

	}

	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
	{
		if (tileMap != nullptr)
		{
			this->tileMap->Draw(viewProjectionMatrix, this->GetOwner()->transform);
			this->bbox->Draw(viewProjectionMatrix);
		}
	}

private:
	TileMap* tileMap = nullptr;

	BoundingBoxRenderer* bbox = nullptr;
	ResourceManager* res = nullptr;
};