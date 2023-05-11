#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class BoundingBoxRenderer;

class Renderer
{
public:
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
	virtual std::string GetPath();
	virtual std::string GetName();
	virtual void InitBoundingBox() {};
	virtual BoundingBoxRenderer* GetBoundingBox() { return nullptr; };

};

