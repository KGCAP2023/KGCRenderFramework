#pragma once
#include "pch.h"
#include "Gameobject.h"
#include "TileMap.h"
#include "Renderer.h"

class TileMapRenderer : public Component , public Renderer
{
public:

	TileMapRenderer(GameObject* owner, ResourceManager* res);

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