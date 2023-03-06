#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Animation3D
{
public:
	Animation3D(const aiScene* pScene)
	{
		this->pScene = pScene;
	}

	void loadAnimation(const aiAnimation* pAnimation);
	const aiNodeAnim* findAnimNode(std::string& key);

private:
	const aiNodeAnim* loadAnimationNode(const std::string& nodeName);

	std::vector<const aiNodeAnim*> nodeAnimations;
	std::map<std::string, UINT> indexAnimation;
	const aiAnimation* pAnimation = nullptr;
	const aiScene* pScene = nullptr;
};

