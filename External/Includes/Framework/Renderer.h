#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Renderer
{
public:
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
	virtual std::string GetPath();

	//Bounding box init
	
	//Bounding box ∏Æ≈œ
};

