#include "pch.h"
#include "TileMapRenderer.h"
#include "ResourceManager.h"
#include "BoundingBox.h"

TileMapRenderer::TileMapRenderer(GameObject* owner, ResourceManager* res) : Component(owner)
{
	this->type = Component::Type::RENDERER_TILEMAP;
	this->name = "TileMapRenderer";
	this->isSpriteRender = true;
	this->res = res;
}

void TileMapRenderer::InitBoundingBox()
{
	this->bbox = new BoundingBox2D(this->owner, this, res);
}

BoundingBoxRenderer* TileMapRenderer::GetBoundingBox()
{
	return this->bbox;
}
