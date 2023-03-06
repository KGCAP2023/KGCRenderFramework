#include "pch.h"
#include "Animation3D.h"

void Animation3D::loadAnimation(const aiAnimation* pAnimation)
{
	this->pAnimation = pAnimation;

	for (UINT i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh* paiMesh = pScene->mMeshes[i];
		for (UINT i = 0; i < paiMesh->mNumBones; i++)
		{
			aiBone* pBone = paiMesh->mBones[i];
			std::string nodeName = pBone->mName.data;
			const aiNodeAnim* pNode = loadAnimationNode(nodeName);

			if (pNode != nullptr)
			{
				nodeAnimations.push_back(pNode);
				indexAnimation.insert(std::make_pair(nodeName, nodeAnimations.size() - 1));
			}

		}
	}
}


const aiNodeAnim* Animation3D::loadAnimationNode(const std::string& nodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == nodeName) {
			return pNodeAnim;
		}
	}
	return nullptr;
}

const aiNodeAnim* Animation3D::findAnimNode(std::string& key)
{
	if (indexAnimation.find(key) != indexAnimation.end()) {
		UINT index = indexAnimation[key];
		return nodeAnimations[index];
	}
	return nullptr;
}
