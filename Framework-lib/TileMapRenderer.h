#pragma once
#include "pch.h"
#include "Gameobject.h"
#include "TileMap.h"

class TileMapRenderer : public Component
{
public:

	TileMapRenderer(GameObject* owner) : Component(owner)
	{
		this->type = Component::Type::RENDERER_TILEMAP;
		this->name = "TileMapRenderer";
		this->isSpriteRender = true;
	}

	void AddTileMap(TileMap* tileMap)
	{
		this->tileMap = tileMap;
	}

	virtual void Update()
	{

	}

	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
	{
		if (tileMap != nullptr)
			tileMap->Draw(viewProjectionMatrix,this->GetOwner()->transform.position);
	}

private:
	TileMap* tileMap = nullptr;
};