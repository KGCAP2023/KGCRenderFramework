#pragma once
#include "pch.h"
#include "Gameobject.h"
#include "Sprite.h"

class TileMap
{
public:
	TileMap(){}
	~TileMap()
	{
		for (int i = 0; i < grid.y; i++)
			delete[] arr[i];
		delete[] arr;
	}

	bool Init(const std::string& name,Sprite* sprite, int gridSizeX, int gridSizeY, int tileSize = 32)
	{
		if (AddTileMapImage(sprite))
		{
			this->name = name;
			this->tileSize = tileSize;

			tile.x = sprite->GetWidth() / tileSize;
			tile.y = sprite->GetHeight() / tileSize;

			grid.x = gridSizeX;
			grid.y = gridSizeY;

			arr = new RECT * [grid.y];

			for (int i = 0; i < grid.y; i++)
				arr[i] = new RECT[grid.x];

			for (int i = 0; i < grid.y; i++)
			{
				for (int j = 0; j < grid.x; j++)
				{
					arr[i][j] = RECT{ -1 , -1 , -1 , -1 };
				}
			}

			return true;
		}
		else
			return false;
	}

	void SelectTile(int tileX, int tileY, int gridX, int gridY)
	{
		if (tileX > tile.x || tileY > tile.y)
			return;

		if (gridX > grid.x || gridY > grid.y)
			return;

		XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
		RECT frame{ (LONG)pos.x + tileX * tileSize, (LONG)pos.y + tileY * tileSize, (LONG)pos.x + (tileX + 1) * tileSize, (LONG)pos.y + (tileY + 1) * tileSize };
		arr[gridX][gridY] = frame;
	}

	void Draw(const DirectX::XMMATRIX& viewProjectionMatrix , Transform& t, float layer)
	{
		XMFLOAT3&  gameObjpos = t.position;
		this->SetScale(t.scale.x);

		if (sprite != nullptr)
		{
			for (int i = 0; i < grid.y; i++)
			{
				for (int j = 0; j < grid.x; j++)
				{
					DirectX::SimpleMath::Vector2 gridPos(gameObjpos.x + j * scale * tileSize, gameObjpos.y + i * scale * tileSize);

					if(arr[i][j].bottom != -1) //undefined case
						spriteBatch->Draw(sprite->Get(), gridPos, &arr[i][j],
							DirectX::Colors::White, t.rotation.z, DirectX::SimpleMath::Vector2(0, 0), scale, SpriteEffects_None, layer);
				}
			}
		}
	}

	std::string GetName()
	{
		return this->name;
	}

	int GetTileSize()
	{
		return this->tileSize;
	}

	void SetScale(float scale)
	{
		this->scale = scale;
	}

	int GetTileMapWidth()
	{
		return grid.x * tileSize;
	}

	int GetTileMapHeight()
	{
		return grid.y * tileSize;
	}

	int GetSpriteWidth()
	{
		return sprite->GetWidth();
	}

	int GetSprtieHeight()
	{
		return sprite->GetHeight();
	}

private:

	bool AddTileMapImage(Sprite* sprite)
	{
		if (sprite != nullptr)
		{
			this->sprite = sprite;
			this->spriteBatch = sprite->GetSpriteBatch();
			return true;
		}
		else
			return false;
	}

	SimpleMath::Vector2 pos;
	SimpleMath::Vector2 grid;
	SimpleMath::Vector2 tile;

	std::string name;
	int tileSize;
	float scale = 1.f;

	RECT** arr;

	Sprite* sprite = nullptr;
	SpriteBatch* spriteBatch = nullptr;

};
