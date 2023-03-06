#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Renderer
{
public:
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
	virtual std::string GetPath();
	virtual const aiScene* GetAiScene();
};

