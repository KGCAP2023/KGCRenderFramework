#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Animation3D
{
public:
	Animation3D(const aiScene* pScene, const std::string& name)
	{
		this->pScene = pScene;
		this->name = name;
	}

	void loadAnimation(const aiAnimation* pAnimation);
	const aiNodeAnim* findAnimNode(std::string& key);
	const aiAnimation* GetAnimation() { return pAnimation; };

private:
	const aiNodeAnim* loadAnimationNode(const std::string& nodeName);
	std::string name;
	std::vector<const aiNodeAnim*> nodeAnimations;
	std::map<std::string, UINT> indexAnimation;
	const aiAnimation* pAnimation = nullptr;
	const aiScene* pScene = nullptr;
};

